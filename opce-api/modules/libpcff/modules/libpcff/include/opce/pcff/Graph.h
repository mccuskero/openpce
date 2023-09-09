/**
 * Copyright (c) 1994-2007 Owen McCusker <pcave@myeastern.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * All rights reserved.
 */
 
/*
*  
*  Graph: Graph implements an abstract data type graph that is 
*  used to manage graph operations.
*  There will be a set of algorithms assoicated with graphs that 
*  operate on the graph.
*  
*  how to create a generic graph behavior, then redefine its
*  behavior based on the type of graph, Forgy, Rete, and so
*  on. Maybe function pointers can be registered to facilitate
*  the addition of "add" functions, based on node type, or
*  graph type.
*
*/

#ifndef Graph_h
#define Graph_h

namespace pcff
{

/*
 * There are many different types of nodes in a graph,
 * which are dependent on the use of the graph.
 */

typedef struct {
   void *info;
   NodeType *point;
   NodeType *next;
} NodeType;

Nodetype *Graph_create();
void      Graph_destroy();

void      Graph_add();

void      Graph_remove();

void      Graph_jointWithWeight( NodeType *point, 
                                 NodeType *next,
                                 int       weight);

int       Graph_adjacent(NodeType *node1,
                         NodeType *node2);

NodeType *Graph_findNode(NodeType *startingPoint,
                         void     *info);

/*
 *  Graph_getNode - called by addNode to create a node on the graph
 *  (where is the node placed?  Is it dependent on the "network"
 *  that is being built.
 */
NodeType *Graph_getNode();

};

#endif
