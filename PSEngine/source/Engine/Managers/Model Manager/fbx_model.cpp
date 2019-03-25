#include "model_manager.h"
#include <fstream>
#include <string>
#include "polyspoon_math.h"
#include "timer.h"
#include "helpers.h"
#include "texture_manager.h"
#include "matrix.h"

// Get the matrix of the given pose
KFbxXMatrix GetPoseMatrix(KFbxPose* pPose, int pNodeIndex)
{
	KFbxXMatrix lPoseMatrix;
	KFbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

	return lPoseMatrix;
}

KFbxXMatrix GetGlobalPosition(KFbxNode* pNode, KTime& pTime, KFbxXMatrix* pParentGlobalPosition)
{
	// Ideally this function would use parent global position and local position to
	// compute the global position.
	// Unfortunately the equation 
	//    lGlobalPosition = pParentGlobalPosition * lLocalPosition
	// does not hold when inheritance type is other than "Parent" (RSrs). To compute
	// the parent rotation and scaling is tricky in the RrSs and Rrs cases.
	// This is why GetGlobalFromCurrentTake() is used: it always computes the right
	// global position.

	return pNode->GetGlobalFromCurrentTake(pTime);
}

void ModelManager::initialise_fbx_sdk_objects(KFbxSdkManager*& sdk_manager_p, KFbxScene*& scene_p)
{
	// The first thing to do is to create the FBX SDK manager which is the 
	// object allocator for almost all the classes in the SDK.
	sdk_manager_p = KFbxSdkManager::Create();

	if (!sdk_manager_p)
	{
		g_console_p->dbgmsg("Unable to create the FBX SDK manager\n");
		return;
	}

	// Load plugins from the executable directory
	KString path = KFbxGetApplicationDirectory();
	KString extension = "dll";
	sdk_manager_p->LoadPluginsDirectory(path.Buffer(), extension.Buffer());

	// Create the entity that will hold the scene.
	scene_p = KFbxScene::Create(sdk_manager_p,"");
}

void ModelManager::init_fbx_manager()
{
	initialise_fbx_sdk_objects(g_fbx_sdk_manager, g_fbx_scene);

	if(g_fbx_sdk_manager)
	{
		g_console_p->dbgmsg("Created FBX SDK manager\n");

		import_fbx_scene();
	}
	else
	{
		return;
	}
}

KArrayTemplate<KString*>   gTakeNameArray;
KTime gayTime;
KTime period;

void ModelManager::initialise_fbx_global_params()
{
	g_console_p->dbgmsg("Initialising global scene parameters\n");

	//setup global params
	KFbxAxisSystem scene_axis_system = g_fbx_scene->GetGlobalSettings().GetAxisSystem();
	KFbxAxisSystem my_axis_system(KFbxAxisSystem::YAxis, KFbxAxisSystem::ParityOdd, KFbxAxisSystem::RightHanded);
	my_axis_system.ConvertScene(g_fbx_scene);

	// Convert Unit System to what is used in this example, if needed
	KFbxSystemUnit scene_system_unit = g_fbx_scene->GetGlobalSettings().GetSystemUnit();
	if( scene_system_unit.GetScaleFactor() != 1.0 )
	{
		KFbxSystemUnit unit_system(1.0);
		unit_system.ConvertScene(g_fbx_scene);
	}

	g_fbx_scene->FillTakeNameArray(gTakeNameArray);
	g_fbx_scene->SetCurrentTake(gTakeNameArray[1]->Buffer());
	KFbxTakeInfo* lCurrentTakeInfo = g_fbx_scene->GetTakeInfo(*(gTakeNameArray[1]));

	period.SetTime(0, 0, 0, 15, 0, g_fbx_scene->GetGlobalTimeSettings().GetTimeMode());

	if (lCurrentTakeInfo)
	{
		gayTime =  lCurrentTakeInfo->mLocalTimeSpan.GetStart();
	}
}

void ModelManager::import_fbx_scene()
{
	int file_format = -1;

	// Create the importer.
	int lFileFormat = -1;
	g_fbx_importer = KFbxImporter::Create(g_fbx_sdk_manager,"");
	if(g_fbx_importer) g_console_p->dbgmsg("Created FBX SDK importer\n");
	else g_console_p->dbgmsg("Unable to create the FBX SDK importer\n");

	std::string filename = "key.fbx";
	filename = MODEL_PATH + filename;

	//detect file format
	if (!g_fbx_sdk_manager->GetIOPluginRegistry()->DetectFileFormat(filename.c_str(), file_format) )
	{
		//try binary format
		file_format = g_fbx_sdk_manager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );
	}

	// initialize the importer
	if(g_fbx_importer->Initialize(filename.c_str(), file_format) == true)
	{
		g_console_p->dbgmsg("\nLoading %s\n", filename.c_str());

		if(g_fbx_importer->Import(g_fbx_scene) == true)
		{
			initialise_fbx_global_params();
		}
	}
}

void ModelManager::render_fbx_scene()
{
	gayTime += period;

	KFbxTakeInfo* lCurrentTakeInfo = g_fbx_scene->GetTakeInfo(*(gTakeNameArray[1]));

	if(gayTime > lCurrentTakeInfo->mLocalTimeSpan.GetStop())
	{
		gayTime =  lCurrentTakeInfo->mLocalTimeSpan.GetStart();
	}

	int child_count = g_fbx_scene->GetRootNode()->GetChildCount();

	m_models.push_back(new Model());

	for(int i = 0; i < child_count; i++)
	{
		render_fbx_node_recursive(g_fbx_scene->GetRootNode()->GetChild(i));
	}
}

KFbxXMatrix GetGeometry(KFbxNode* pNode)
{
	KFbxVector4 lT, lR, lS;
	KFbxXMatrix lGeometry;

	lT = pNode->GetGeometricTranslation(KFbxNode::eSOURCE_SET);
	lR = pNode->GetGeometricRotation(KFbxNode::eSOURCE_SET);
	lS = pNode->GetGeometricScaling(KFbxNode::eSOURCE_SET);

	lGeometry.SetT(lT);
	lGeometry.SetR(lR);
	lGeometry.SetS(lS);

	return lGeometry;
}

void ModelManager::render_fbx_node_recursive(KFbxNode* pNode)
{
	// Geometry offset.
	// it is not inherited by the children.
	KFbxXMatrix lGlobalPosition = pNode->GetGlobalFromCurrentTake(gayTime);

	KFbxXMatrix lGeometryOffset = GetGeometry(pNode);
	KFbxXMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

	render_fbx_node(pNode, lGlobalOffPosition);

	int i, lCount = pNode->GetChildCount();
	for (i = 0; i < lCount; i++)
	{
		render_fbx_node_recursive(pNode->GetChild(i));
	}
}

void ModelManager::render_fbx_node( KFbxNode* pNode, KFbxXMatrix& pGlobalPosition)
{
	KFbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute)
	{
		if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eMESH)
		{
			render_fbx_mesh(pNode, pGlobalPosition);
		}
		else if(lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eSKELETON)
		{
		}
	}
}

// Scale all the elements of a matrix.
void MatrixScale(KFbxXMatrix& pMatrix, double pValue)
{
	int i,j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			pMatrix[i][j] *= pValue;
		}
	}
}


// Add a value to all the elements in the diagonal of the matrix.
void MatrixAddToDiagonal(KFbxXMatrix& pMatrix, double pValue)
{
	pMatrix[0][0] += pValue;
	pMatrix[1][1] += pValue;
	pMatrix[2][2] += pValue;
	pMatrix[3][3] += pValue;
}


// Sum two matrices element by element.
void MatrixAdd(KFbxXMatrix& pDstMatrix, KFbxXMatrix& pSrcMatrix)
{
	int i,j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			pDstMatrix[i][j] += pSrcMatrix[i][j];
		}
	}
}

//some stuff i will need for anims

/*--------------------------------------------------------------*\

//skin count = skeletons

//cluster count = bones or clusters of vertices affected by bones

//lReferenceGlobalCurrentPosition is global position
//need to eliminate this by inverse

//this is the initial position of the bone
//lCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);

//


\*--------------------------------------------------------------*/

// Deform the vertex array with the links contained in the mesh.
void ComputeClusterDeformation(KFbxXMatrix& pGlobalPosition, 
							   KFbxMesh* pMesh, 
							   KTime& pTime, 
							   KFbxVector4* pVertexArray,
							   KFbxPose* pPose,
							   Matrix4Array *myMat,
							   WeightArray *weight)
{
	// All the links must have the same link mode.
	KFbxCluster::ELinkMode lClusterMode = ((KFbxSkin*)pMesh->GetDeformer(0, KFbxDeformer::eSKIN))->GetCluster(0)->GetLinkMode();

	int i, j;
	int lClusterCount=0;

	//int lLinkCount = pMesh->GetLinkCount();
	int lVertexCount = pMesh->GetControlPointsCount();

	KFbxXMatrix* lClusterDeformation = new KFbxXMatrix[lVertexCount];
	memset(lClusterDeformation, 0, lVertexCount * sizeof(KFbxXMatrix));
	double* lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

	int lSkinCount=pMesh->GetDeformerCount(KFbxDeformer::eSKIN);

	//if (lLinkMode == KFbxLink::eADDITIVE)
	if(lClusterMode == KFbxCluster::eADDITIVE)
	{   
		for (i = 0; i < lVertexCount; i++)
		{   
			lClusterDeformation[i].SetIdentity();
		}
	}

	for(i=0; i<lSkinCount; ++i)
	{
		lClusterCount =( (KFbxSkin *)pMesh->GetDeformer(i, KFbxDeformer::eSKIN))->GetClusterCount();
		for (j=0; j<lClusterCount; ++j)
		{
			KFbxCluster* lCluster =((KFbxSkin *) pMesh->GetDeformer(i, KFbxDeformer::eSKIN))->GetCluster(j);
			if (!lCluster->GetLink())
				continue;
			KFbxXMatrix lReferenceGlobalInitPosition;
			KFbxXMatrix lReferenceGlobalCurrentPosition;
			KFbxXMatrix lClusterGlobalInitPosition;
			KFbxXMatrix lClusterGlobalCurrentPosition;
			KFbxXMatrix lReferenceGeometry;
			KFbxXMatrix lClusterGeometry;

			KFbxXMatrix lClusterRelativeInitPosition;
			KFbxXMatrix lClusterRelativeCurrentPositionInverse;
			KFbxXMatrix lVertexTransformMatrix;

			lCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
			lReferenceGlobalCurrentPosition = pGlobalPosition;

			// Multiply lReferenceGlobalInitPosition by Geometric Transformation
			lReferenceGeometry = GetGeometry(pMesh->GetNode());
			lReferenceGlobalInitPosition *= lReferenceGeometry;

			// Get the link initial global position and the link current global position.
			lCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
			lClusterGlobalCurrentPosition = GetGlobalPosition(lCluster->GetLink(), pTime, NULL);

			// Compute the initial position of the link relative to the reference.
			lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

			// Compute the current position of the link relative to the reference.
			lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

			// Compute the shift of the link relative to the reference.
			lVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;

			int k;
			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();

			for (k = 0; k < lVertexIndexCount; ++k) 
			{            
				int lIndex = lCluster->GetControlPointIndices()[k];
				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0)
				{
					continue;
				}

				// Compute the influence of the link on the vertex.
				KFbxXMatrix lInfluence = lVertexTransformMatrix;
				MatrixScale(lInfluence, lWeight);

				// Add to the sum of the deformations on the vertex.
				MatrixAdd(lClusterDeformation[lIndex], lInfluence);

				// Add to the sum of weights to either normalize or complete the vertex.
				lClusterWeight[lIndex] += lWeight;
			}
		}
	}

	for (i = 0; i < lVertexCount; i++) 
	{
		KFbxVector4 lSrcVertex = pVertexArray[i];
		KFbxVector4& lDstVertex = pVertexArray[i];
		double lWeight = lClusterWeight[i];

		Matrix4 newmat;
		newmat.set_matrix_from_gl( (double *)lClusterDeformation[i] );
		myMat->push_back(newmat);

		weight->push_back(lWeight);

		// Deform the vertex if there was at least a link with an influence on the vertex,
		if (lWeight != 0.0) 
		{
			lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);
			
			if (lClusterMode == KFbxCluster::eNORMALIZE)
			{
				// In the normalized link mode, a vertex is always totally influenced by the links. 
				lDstVertex /= lWeight;
			}
			else if (lClusterMode == KFbxCluster::eTOTAL1)
			{
				// In the total 1 link mode, a vertex can be partially influenced by the links. 
				lSrcVertex *= (1.0 - lWeight);
				lDstVertex += lSrcVertex;
			}
		} 
	}

	delete [] lClusterDeformation;
	delete [] lClusterWeight;
}

int m_tcount = 0;
KArrayTemplate<KString*> namearr;

void ModelManager::render_fbx_mesh( KFbxNode* pNode, KFbxXMatrix& pGlobalPosition)
{
	KFbxMesh* lMesh = (KFbxMesh*) pNode->GetNodeAttribute();
	int lClusterCount = 0;
	int lSkinCount= 0;
	int lVertexCount = lMesh->GetControlPointsCount();

	// Create a copy of the vertex array to receive vertex deformations.
	KFbxVector4* lVertexArray = new KFbxVector4[lVertexCount];
	memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(KFbxVector4));

	KFbxLayerElementArrayTemplate<KFbxVector2>* lUVArray = NULL;    
	lMesh->GetTextureUV(&lUVArray, KFbxLayerElement::eDIFFUSE_TEXTURES); 

	int lPolygonCount = lMesh->GetPolygonCount();

	Vector3fArray vbo_vertices;
	Vector3fArray vbo_normals;
	Vector2fArray vbo_texcoords;
	Vector4fArray vbo_tangents;

	Matrix4Array deforms;
	WeightArray weight;

	//we need to get the number of clusters
	lSkinCount = lMesh->GetDeformerCount(KFbxDeformer::eSKIN);
	for( int i=0; i< lSkinCount; i++)
		lClusterCount += ((KFbxSkin *)(lMesh->GetDeformer(i, KFbxDeformer::eSKIN)))->GetClusterCount();
	if (lClusterCount)
	{
		// Deform the vertex array with the links.
		ComputeClusterDeformation(pGlobalPosition, lMesh, gayTime, lVertexArray, NULL, &deforms, &weight);
	}

	for(int i = 0; i < lPolygonCount; i++)
	{
		int lVertexCount = lMesh->GetPolygonSize(i);

		for(int v = 0; v < lVertexCount; v++)
		{
			float x = lVertexArray[lMesh->GetPolygonVertex(i, v)].GetAt(0);
			float y = lVertexArray[lMesh->GetPolygonVertex(i, v)].GetAt(1);
			float z = lVertexArray[lMesh->GetPolygonVertex(i, v)].GetAt(2);

			KFbxVector4 norm;
			lMesh->GetPolygonVertexNormal(i, v, norm);

			float nx = norm.GetAt(0);
			float ny = norm.GetAt(1);
			float nz = norm.GetAt(2);

			int lCurrentUVIndex = lMesh->GetTextureUVIndex(i, v);

			float s = lUVArray->GetAt(lCurrentUVIndex).GetAt(0);
			float t = lUVArray->GetAt(lCurrentUVIndex).GetAt(1);

			Vector3f vertr(x,z,-y);
			Vector3f nnn(nx,nz,-ny);

			//try doing some kind of transform
			Matrix4 psmat;
			psmat.set_matrix_from_gl((double *)pGlobalPosition);
			Matrix4 psrot = psmat.get_orientation();

			Vector3f vert(x,y,z);
			if(deforms.size() > 0 && weight[v] > 0.0f) vert = deforms[v] * vert;
			vert = psmat * vert;

			

			Vector3f realn(nx,ny,nz);
			realn = psrot * realn;
			if(deforms.size() > 0 && weight[v] > 0.0f) 
			{
				Matrix4 dsrot = deforms[v].get_orientation();
				vert = dsrot * vert;
			}
			realn.normalise();

			vbo_vertices.push_back(vert);
			vbo_normals.push_back(realn);
			vbo_texcoords.push_back(Vector2f(s,t));
		}
	}

	/*glBegin(GL_TRIANGLES);

		for (int i = 0; i < vbo_vertices.size(); i++)
		{
			glVertex3f(vbo_vertices.at(i).x,vbo_vertices.at(i).y,vbo_vertices.at(i).z);
		}

	glEnd();

	return;*/

	vbo_tangents = calculate_tangents(vbo_vertices, vbo_texcoords, vbo_normals);

	int lMaterialIndex;
	int lTextureIndex;
	KFbxProperty lProperty;
	int lNbTex;
	KFbxTexture* lTexture = NULL; 
	KFbxSurfaceMaterial *lMaterial = NULL;
	int lNbMat = pNode->GetSrcObjectCount(KFbxSurfaceMaterial::ClassId);

	char* tname = NULL;

	for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++)
	{
		lMaterial = KFbxCast <KFbxSurfaceMaterial>(pNode->GetSrcObject(KFbxSurfaceMaterial::ClassId, lMaterialIndex));
		if(lMaterial)
		{                                                          
			lProperty = lMaterial->FindProperty(KFbxSurfaceMaterial::sDiffuse);
			if(lProperty.IsValid())
			{
				lNbTex = lProperty.GetSrcObjectCount(KFbxTexture::ClassId);

				KFbxTexture* lCurrentTexture = KFbxCast <KFbxTexture>(lProperty.GetSrcObject(KFbxTexture::ClassId, 0)); 

				if(lCurrentTexture)
				{
					tname = const_cast<char *>(lCurrentTexture->GetFileName());
				}
			}
		}
	}

	char stiped[32];
	char stiped_n[32];
	//strip out the shit
	int len = strlen(tname) - 1;

	int count = -1;
	for(int i = len; i > 0; i--)
	{
		if(tname[i] == '/' || tname[i] == '\\')
		{
			i = -1;
		}
		else
		{
			count++;
		}
	}

	int count2 = 0;
	for(int i = len - count; i <= len; i++)
	{
		stiped[count2++] = tname[i];
	}

	stiped[count2] = '\0';

	strcpy(stiped_n,"n_");
	strcat(stiped_n,stiped);

	//anything left can be bunged into a mesh
	int id = m_models.size() - 1;

	Material *add_mat = new Material("t");
	Mesh *add_mesh = new Mesh();

	add_mat->load_texture(stiped,DIFFUSE);
	add_mat->load_texture(stiped_n,NORMAL);

	if(tname)
	{
		m_models.at(id)->add_material(add_mat);
	}

	add_mesh->add_vertices(vbo_vertices);
	add_mesh->add_texture_coords(vbo_texcoords);
	add_mesh->add_normals(vbo_normals);
	add_mesh->add_tangents(vbo_tangents);
	//add_mesh->add_bone_matrix(deforms, weight);

	m_models.at(id)->add_mesh(add_mesh,m_tcount);
	m_models.at(id)->calculate_aabb();

	m_tcount++;
}
