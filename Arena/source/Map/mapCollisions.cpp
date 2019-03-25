#include "map.h"
#include "modelManager.h"
#include "HCI.h"

void CMap::collisionHandler(CPlayer *player)
{
	SVector3 shotIntersection = player->shotline[1];
	player->setCrosshair(shotIntersection);

	player->updateBullets();

	//Check Bullets
	int equipped = player->m_equippedGun;
	for(int j = 0; j < MAX_BULLETS; j++)
	{
		if(player->myGun[equipped]->bullets[j].alive)
		{
			bulletCollision(&player->myGun[equipped]->bullets[j]);
		}
	}

	for(int i = 0; i < 4; i++)
	{
		pickups[i].check(*player);
	}
}

void CMap::newBulletHole(SVector3 position, STriangle collisionTri, SVector3 polysNormal, int bulletType)
{
	bulletHole[m_numberOfBulletHoles].active = true;
	bulletHole[m_numberOfBulletHoles].position = position;

	SVector3 point[4];

	for(int i = 0; i < 3; i++) 
	{
		SVector3 temp = normalize(collisionTri.point[i]) * 5;
		point[i].x = temp.x;
		point[i].z = temp.y;
		point[i].y = temp.z;
	}

	point[3] = point[0];

	SVector3 newNorm = getNormal(point);

	SVector3 yrotNormal = newNorm;
	SVector3 xrotNormal = newNorm;

	xrotNormal.x = 0;
	xrotNormal.y = 0;
	yrotNormal.y = 0;

	SVector3 up(0,1,0);
	SVector3 front(0,0,-1);

	float xr = -asin(newNorm.y);
	float yr = angleBetweenVector(yrotNormal,front);

	if(front.x > yrotNormal.x)
	{
		yr *= -1;
	}

	bulletHole[m_numberOfBulletHoles].rotation.x = xr * 180 / PI;
	bulletHole[m_numberOfBulletHoles].rotation.y = yr * 180 / PI;
	m_numberOfBulletHoles++;

	for(i = 0; i < 10; i++)
	{
		if(!particleSystem[i].inUse) 
		{
			particleSystem[i].initRicochet(position,bulletType);
			i = 10;
		}
	}

}

void CMap::bulletCollision(SBullet *bullet)
{
	SVector3 impactPoint(0,0,0);
	STriangle bulletHoleTri;
	SVector3 normal;

		if(m_meshID[COLLISION_BULLETS] != -1)
		{	
			if(CModelManager::getInstance()->lineCollision(bullet->line,m_meshID[COLLISION_BULLETS],&impactPoint,&bulletHoleTri, &normal))
			{
				bullet->alive = false;

				newBulletHole(impactPoint,bulletHoleTri,normal,bullet->type);
			}
		}

}

void CMap::checkCollision(CPlayer *player)
{
	int accuracy = 18;
	SVector3 velocityToSpend = player->getReadyForCollision();
	SVector3 velocitySpent = SVector3(0,0,0);
	bool axisComplete[3] = {0,0,0};
	bool finished = false;
	bool portal = false;

		do
		{
			//increment the player along the move vector
			SVector3 thisPush;
			SVector3 pos = player->posVector();

			//x-axis
			if(velocityToSpend.x - velocitySpent.x > accuracy)
			{
				
				velocitySpent.x += accuracy;
				thisPush.x = accuracy;
			}
			else if (velocityToSpend.x - velocitySpent.x < -accuracy)
			{
				velocitySpent.x -= accuracy;
				thisPush.x = -accuracy;
			}
			else
			{
				float remainder  =  velocityToSpend.x - velocitySpent.x;
				velocitySpent.x += remainder;
				thisPush.x = remainder;

				if(remainder == 0) axisComplete[0] = true;
			}

			//y-axis
			if(velocityToSpend.y - velocitySpent.y > accuracy)
			{
				velocitySpent.y += accuracy;
				thisPush.y = accuracy;
			}
			else if (velocityToSpend.y - velocitySpent.y < -accuracy)
			{
				velocitySpent.y -= accuracy;
				thisPush.y = -accuracy;
			}
			else
			{
				float remainder  =  velocityToSpend.y - velocitySpent.y;
				velocitySpent.y += remainder;
				thisPush.y = remainder;

				if(remainder == 0) axisComplete[1] = true;
			}

			//z-axis
			if(velocityToSpend.z - velocitySpent.z > accuracy)
			{
				velocitySpent.z += accuracy;
				thisPush.z = accuracy;
			}
			else if (velocityToSpend.z - velocitySpent.z < -accuracy)
			{
				velocitySpent.z -= accuracy;
				thisPush.z = -accuracy;
			}
			else
			{
				float remainder  =  velocityToSpend.z - velocitySpent.z;
				velocitySpent.z += remainder;
				thisPush.z = remainder;


				if(remainder == 0) 
				{
					axisComplete[2] = true;
				}
			}

			thisPush.y = (int)thisPush.y;

			player->moveVector(thisPush);

			if(!finished)
			{
				if(m_meshID[COLLISION_OFFSET] != -1)
				{
					CModelManager::getInstance()->checkAllCollision(player,m_meshID[COLLISION_OFFSET],true);
				}

				if(m_meshID[COLLISION_BLOCKING] != -1)
				{
					CModelManager::getInstance()->checkAllCollision(player,m_meshID[COLLISION_BLOCKING],false);
				}
			}

			//check if the recursion is complete
			if((axisComplete[0]) && (axisComplete[1]) && (axisComplete[2]))
			{	
				finished = true;
			}

			
		}while(!finished);

		player->updatePos();
		player->setPositionData();
}

void CMap::bulletVsPlayer(CPlayer *collider, CPlayer *collidee,bool reduce)
{
	SVector3 temp(0,0,0);
	int equippedGun = collider->m_equippedGun;

	for(int j = 0; j < MAX_BULLETS; j++)
	{
		if((collider->myGun[equippedGun]->bullets[j].alive) && (collidee->m_alive))
		{
			if(collidee->checkForHit(collider->myGun[equippedGun]->bullets[j].line,&temp))
			{
					if(reduce)
					{
						collidee->reduceHealth(collider->myGun[equippedGun]->returnDamage());
					}
					collider->myGun[equippedGun]->bullets[j].alive = false;
					collidee->initDamageIndicator(collider->yRotation());

					for(int i = 0; i < 10; i++)
					{
						if(!bloodParticles[i].inUse) 
						{
							bloodParticles[i].initBlood(temp);
							i = 10;
						}
					}
			}
		}
	}

	SVector3 cursorPos[2];
	cursorPos[0] = collider->shotline[1];
	cursorPos[1] = collider->shotline[1];

	if(collidee->checkForHit(cursorPos,&temp))
	{
		collider->m_crosshairOverPlayer = true;
	}
	else
	{
		collider->m_crosshairOverPlayer = false;
	}
}

void CMap::playerVsPlayer(CPlayer *collider, CPlayer *collidee)
{
	collider->checkCollision(collidee->posVector(),collidee->radiusVector());
}