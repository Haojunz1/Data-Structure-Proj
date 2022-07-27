#include "QuadTree.h"

using namespace std;

QuadTree :: iterator ::	iterator(){
  m_index = 0;
}

QuadTree :: iterator ::	~iterator(){
  
}

bool QuadTree :: iterator :: operator==(const QuadTree::iterator &other){
  if (m_vectorInIterator == other.m_vectorInIterator && m_index == other.m_index) {
    return true;
  } else {
    return false;
  }
}

bool QuadTree :: iterator :: operator!=(const QuadTree::iterator &other){
  if (m_vectorInIterator != other.m_vectorInIterator && m_index != other.m_index) {
    return true;
  } else {
    return false;
  }
}

// Following two operators move on to next non-zero cell
QuadTree ::	iterator &QuadTree::iterator:: operator++(){
  m_index++;
  return *this;
}          
// Prefix: e.g. "++it"
QuadTree :: iterator QuadTree::iterator:: operator++(int dummy){
  QuadTree::iterator it;
  it.m_index = this -> m_index;
  it.m_vectorInIterator = this -> m_vectorInIterator;
  m_index++;
  return it;
} 

QuadTree::iterator::iterator(vector<Point> m_vector, int index) {
  m_index = index;
  m_vectorInIterator = m_vector;
}
// Postfix: "it++"
Point &QuadTree :: iterator:: operator*(){
  return m_vectorInIterator[m_index];
}

const QuadTree :: iterator &QuadTree::iterator::operator=(const iterator &rhs){
  m_index = rhs.m_index;
  m_vectorInIterator = rhs.m_vectorInIterator;
  return *this;
}

// Now, the stuff for the main class:
//
// Creates a 16x16 default board
QuadTree :: QuadTree(){
  m_root = new QTQuad();
  m_bounds = BBox(Point(0,0), 16);
  m_qBounds = m_bounds;
} 

QuadTree :: QuadTree(const BBox &bounds){
  m_root = new QTQuad();
  m_bounds = bounds;
  m_qBounds = BBox(Point(bounds.m_bL.m_x,bounds.m_bL.m_y), BBox::roundUpPow2(bounds.m_dim));
}

QuadTree :: ~QuadTree(){
  if (m_root != NULL) {
    delete m_root;
    m_root = NULL;
  }
}

// Retrieve cell value
int QuadTree :: get(const Point &pt){
  return m_root -> get(pt, m_qBounds);
}

// Set cell value
void QuadTree :: set(const Point &pt, int data){
  m_root -> set(pt, data, m_qBounds);
}

// Modify cell value by a signed delta amount; faster then get() + set()
int QuadTree :: increment(const Point &pt, int delta) {
  return m_root -> increment(pt, delta, m_qBounds);
}
// Clear all cells in tree
void QuadTree :: clearAll(){
  m_root -> pruning(m_qBounds);
}

void QuadTree :: treverse() {
  m_root -> treverseTree(m_qBounds);
}

// return iterator to first non-zero cell; == end() if qtree empty
QuadTree :: iterator QuadTree:: begin(){
  treverse();
  return QuadTree::iterator(QTQuad::m_vector_quad, 0);
}

// iterator pointing beyond last cell;
QuadTree :: iterator QuadTree :: end(){
  treverse();
  return QuadTree::iterator(QTQuad::m_vector_quad, QTQuad::m_vector_quad.size());
}




