#ifndef INC_NODE_H
#define INC_NODE_H

#define NODENOTHING		0
#define NODESTART		1
#define NODEFINISH		2
#define NODEWALL		3
#define NODEOPEN		4
#define NODECLOSE		5

class CNode {
public:
	CNode();
	~CNode();
	unsigned int Fn;
	unsigned int Gn;
	unsigned int Hn;
	unsigned int x;
	unsigned int y;
	CNode* m_parentNode;
	unsigned int calculateFn();
	unsigned int calculateGn(CNode* neighNode);
	unsigned int calculateHn(CNode* targetNode);
};

#endif /* INC_NODE_H */
