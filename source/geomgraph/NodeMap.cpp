/**********************************************************************
 * $Id$
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.refractions.net
 *
 * Copyright (C) 2001-2002 Vivid Solutions Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation. 
 * See the COPYING file for more information.
 *
 **********************************************************************
 * $Log$
 * Revision 1.2  2004/07/02 13:28:26  strk
 * Fixed all #include lines to reflect headers layout change.
 * Added client application build tips in README.
 *
 * Revision 1.1  2004/03/19 09:48:45  ybychkov
 * "geomgraph" and "geomgraph/indexl" upgraded to JTS 1.4
 *
 * Revision 1.12  2003/11/07 01:23:42  pramsey
 * Add standard CVS headers licence notices and copyrights to all cpp and h
 * files.
 *
 *
 **********************************************************************/


#include <geos/geomgraph.h>

namespace geos {

NodeMap::NodeMap(NodeFactory *newNodeFact) {
	nodeFact=newNodeFact;
	nodeMap=new map<Coordinate,Node*,CoordLT>();
}

NodeMap::~NodeMap() {
	map<Coordinate,Node*,CoordLT>::iterator	it=nodeMap->begin();
	for (;it!=nodeMap->end();it++) {
		Node *node=it->second;
		delete node;
	}
	delete nodeMap;
	delete nodeFact;
}

Node* NodeMap::addNode(const Coordinate& coord){
	Node *node=find(coord);
	if (node==NULL) {
		node=nodeFact->createNode(coord);
		(*nodeMap)[coord]=node;
	}
	return node;
}

// first arg cannot be const because
// it is liable to label-merging ... --strk;
Node* NodeMap::addNode(Node *n){
	Node *node=find(n->getCoordinate());
	if (node==NULL) {
		(*nodeMap)[n->getCoordinate()]=n;
		return n;
	}
	node->mergeLabel(n);
	return node;
}

void NodeMap::add(EdgeEnd *e) {
	Coordinate& p=e->getCoordinate();
	Node *n=addNode(p);
	n->add(e);
}

/**
 * @return the node if found; null otherwise
 */
Node* NodeMap::find(const Coordinate& coord) const {
	map<Coordinate,Node*,CoordLT>::iterator found=nodeMap->find(coord);
	if (found==nodeMap->end())
		return NULL;
	else
		return found->second;
}

map<Coordinate,Node*,CoordLT>::iterator NodeMap::iterator() const {
	return nodeMap->begin();
}

//Doesn't work yet. Use iterator.
//public Collection NodeMap::values(){
//	return nodeMap.values();
//}

vector<Node*>* NodeMap::getBoundaryNodes(int geomIndex) const {
	vector<Node*>* bdyNodes=new vector<Node*>();
	map<Coordinate,Node*,CoordLT>::iterator	it=nodeMap->begin();
	for (;it!=nodeMap->end();it++) {
		Node *node=it->second;
		if (node->getLabel()->getLocation(geomIndex)==Location::BOUNDARY)
			bdyNodes->push_back(node);
	}
	return bdyNodes;
}

string NodeMap::print() const {
	string out="";
	map<Coordinate,Node*,CoordLT>::iterator	it=nodeMap->begin();
	for (;it!=nodeMap->end();it++) {
		Node *node=it->second;
		out+=node->print();
	}
	return out;
}
}

