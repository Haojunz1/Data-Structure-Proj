//File: QTQuad.cpp

#include "QTQuad.h"

using namespace std;
vector<Point> QTQuad::m_vector_quad;

QTQuad::QTQuad(){
  for (int i = 0; i < QTQ_NUM_QUADS; i++) {
    m_quads[i] = NULL;
  }
  m_cells = NULL;
  m_isValid = true;
}

QTQuad::~QTQuad(){
  if (m_cells != NULL) {
    delete m_cells;
    m_cells = NULL;
  }
  for (int i = 0; i < QTQ_NUM_QUADS; i++) {
    if (m_quads[i] != NULL) {
      delete m_quads[i];
      m_quads[i] = NULL;
    }
  }
}

int QTQuad::get(const Point &pt, const BBox &bounds){
  if (bounds.inBounds(pt) == false) {
    return 0;
  }
  if (bounds.m_dim == QTQ_GRID_DIM) {
    if (m_cells == NULL) {
      return 0;
    }
    int actualX = pt.m_x - bounds.m_bL.m_x;
    int actualY = pt.m_y - bounds.m_bL.m_y;
    return m_cells[actualX][actualY];
  }
  
  int newDim = bounds.m_dim / 2;
  int q1x = bounds.m_bL.m_x;
  int q1y = bounds.m_bL.m_y;
  int q2x = bounds.m_bL.m_x + newDim;
  int q2y = bounds.m_bL.m_y;
  int q3x = bounds.m_bL.m_x;
  int q3y = bounds.m_bL.m_y + newDim;
  int q4x = bounds.m_bL.m_x + newDim;
  int q4y = bounds.m_bL.m_y + newDim;
  
  BBox q1Bounds = BBox(Point(q1x,q1y), newDim);
  BBox q2Bounds = BBox(Point(q2x,q2y), newDim);
  BBox q3Bounds = BBox(Point(q3x,q3y), newDim);
  BBox q4Bounds = BBox(Point(q4x,q4y), newDim);
  
  if (q1Bounds.inBounds(pt) == true && m_quads[0] != NULL) {
    return m_quads[0] -> get(pt, q1Bounds);
  }
  
  else if (q2Bounds.inBounds(pt) == true && m_quads[1] != NULL) {
    return m_quads[1] -> get(pt, q2Bounds);
  }
    
  else if (q3Bounds.inBounds(pt) == true && m_quads[2] != NULL) {
    return m_quads[2] -> get(pt, q3Bounds);
  }
  
  else if (q4Bounds.inBounds(pt) == true && m_quads[3] != NULL) {
    return m_quads[3] -> get(pt, q4Bounds);
  } 
  
  
  
}

void QTQuad::set(const Point &pt, int data, const BBox &bounds) {
  if (bounds.m_dim == QTQ_GRID_DIM) {
    if (m_cells == NULL) {
      m_cells = new int[QTQ_GRID_DIM][QTQ_GRID_DIM];
      for (int i = 0; i < QTQ_GRID_DIM; i++) {
	for (int j = 0; j < QTQ_GRID_DIM; j++) {
	  m_cells[i][j] = 0;
	}
      }
    }
    int actualX = pt.m_x - bounds.m_bL.m_x;
    int actualY = pt.m_y - bounds.m_bL.m_y;
    m_cells[actualX][actualY] = data;
    if (data == 0) {
      bool isValid = false;
      for (int i = 0; i < QTQ_GRID_DIM; i++) {
	for (int j = 0; j < QTQ_GRID_DIM; j++) {
	  if (m_cells[i][j] != 0) {
	    isValid = true;
	    break;
	  }
	}
      }
      if (isValid == false) {
	delete [] m_cells;
	m_cells = NULL;
      }
    }
    return;
  }
  
  int newDim = bounds.m_dim / 2;
  int q1x = bounds.m_bL.m_x;
  int q1y = bounds.m_bL.m_y;
  int q2x = bounds.m_bL.m_x + newDim;
  int q2y = bounds.m_bL.m_y;
  int q3x = bounds.m_bL.m_x;
  int q3y = bounds.m_bL.m_y + newDim;
  int q4x = bounds.m_bL.m_x + newDim;
  int q4y = bounds.m_bL.m_y + newDim;
 
  BBox q1Bounds = BBox(Point(q1x,q1y), newDim);
  BBox q2Bounds = BBox(Point(q2x,q2y), newDim);
  BBox q3Bounds = BBox(Point(q3x,q3y), newDim);
  BBox q4Bounds = BBox(Point(q4x,q4y), newDim);
    
  if (q1Bounds.inBounds(pt) == true) {
    if (m_quads[0] == NULL) {
      m_quads[0] = new QTQuad();
    }
    m_quads[0] -> set(pt, data, q1Bounds);
    if (newDim == 4 && m_quads[0] -> m_cells == NULL) {
      delete m_quads[0];
      m_quads[0] = NULL;
    }
  }
  
  else if (q2Bounds.inBounds(pt) == true) {
    if (m_quads[1] == NULL) {
      m_quads[1] = new QTQuad();
    }
    m_quads[1] -> set(pt, data, q2Bounds);
    if (newDim == 4 && m_quads[1] -> m_cells == NULL) {
      delete m_quads[1];
      m_quads[1] = NULL;
    }
  }
  
  else if (q3Bounds.inBounds(pt) == true) {
    if (m_quads[2] == NULL) {
      m_quads[2] = new QTQuad();
    }
    m_quads[2] -> set(pt, data, q3Bounds);
    if (newDim == 4 && m_quads[2] -> m_cells == NULL) {
      delete m_quads[2];
      m_quads[2] = NULL;
    }
  }
  
  else if (q4Bounds.inBounds(pt) == true) {
    if (m_quads[3] == NULL) {
      m_quads[3] = new QTQuad();
    }
    m_quads[3] -> set(pt, data, q4Bounds);
    if (newDim == 4 && m_quads[3] -> m_cells == NULL) {
      delete m_quads[3];
      m_quads[3] = NULL;
    }
  }
  
  
  //check the child;
  if (bounds.m_dim != QTQ_GRID_DIM) {
        for (int i = 0; i < QTQ_NUM_QUADS; i++) {
	  if (bounds.m_dim > QTQ_GRID_DIM * 2 && m_quads[i] != NULL && m_quads[i] -> m_isValid == false) {
	    delete m_quads[i];
	    m_quads[i] = NULL;
	  }
        }
  }
  
  //check the current
  if (bounds.m_dim != QTQ_GRID_DIM) {
    m_isValid = false;
    for (int i = 0; i < QTQ_NUM_QUADS; i++) {
      if (m_quads[i] != NULL) {
	m_isValid = true;
            }
    }
  }
  
  
}

int QTQuad::increment(const Point &pt, int delta, const BBox &bounds) {
  if (bounds.m_dim == QTQ_GRID_DIM) {
    int actualX = pt.m_x - bounds.m_bL.m_x;
    int actualY = pt.m_y - bounds.m_bL.m_y;
    int newData = m_cells[actualX][actualY] + delta;
    cout << "newdata is " << newData << endl;
    m_cells[actualX][actualY] = newData;
    
    if (newData <= 0) {
      bool isValid = false;
      for (int i = 0; i < QTQ_GRID_DIM; i++) {
	for (int j = 0; j < QTQ_GRID_DIM; j++) {
	  if (m_cells[i][j] != 0) {
	    isValid = true;
	    break;
	  }
	}
      }
      if (isValid == false) {
	delete [] m_cells;
	m_cells = NULL;
          }
    }
    
    return newData;
  }
  
  int newDim = bounds.m_dim / 2;
  int q1x = bounds.m_bL.m_x;
  int q1y = bounds.m_bL.m_y;
  int q2x = bounds.m_bL.m_x + newDim;
  int q2y = bounds.m_bL.m_y;
  int q3x = bounds.m_bL.m_x;
  int q3y = bounds.m_bL.m_y + newDim;
  int q4x = bounds.m_bL.m_x + newDim;
  int q4y = bounds.m_bL.m_y + newDim;

  
  BBox q1Bounds = BBox(Point(q1x,q1y), newDim);
  BBox q2Bounds = BBox(Point(q2x,q2y), newDim);
  BBox q3Bounds = BBox(Point(q3x,q3y), newDim);
  BBox q4Bounds = BBox(Point(q4x,q4y), newDim);
  
  if (q1Bounds.inBounds(pt) == true) {
    if (m_quads[0] == NULL) {
      m_quads[0] = new QTQuad();
    }
    m_quads[0] -> increment(pt, delta, q1Bounds);
    if (newDim == 4 && m_quads[0] -> m_cells == NULL) {
      delete m_quads[0];
      m_quads[0] = NULL;
    }
  }
  
  else if (q2Bounds.inBounds(pt) == true) {
    if (m_quads[1] == NULL) {
      m_quads[1] = new QTQuad();
    }
    m_quads[1] -> increment(pt, delta, q2Bounds);
        if (newDim == 4 && m_quads[1] -> m_cells == NULL) {
	  delete m_quads[1];
	  m_quads[1] = NULL;
        }
  }
  
  else if (q3Bounds.inBounds(pt) == true) {
    if (m_quads[2] == NULL) {
      m_quads[2] = new QTQuad();
    }
    m_quads[2] -> increment(pt, delta, q3Bounds);
    if (newDim == 4 && m_quads[2] -> m_cells == NULL) {
      delete m_quads[2];
      m_quads[2] = NULL;
    }
  }
  
  else if (q4Bounds.inBounds(pt) == true) {
    if (m_quads[3] == NULL) {
      m_quads[3] = new QTQuad();
    }
    m_quads[3] -> increment(pt, delta, q4Bounds);
    if (newDim == 4 && m_quads[3] -> m_cells == NULL) {
      delete m_quads[3];
      m_quads[3] = NULL;
    }
  }
  
}

void QTQuad::clearAll(){
  
}


void QTQuad::treverseTree(const BBox &bounds) {
  if (bounds.m_dim == QTQ_GRID_DIM) {
    if (m_cells != NULL) {
      for (int i = 0; i < QTQ_GRID_DIM; i++) {
	for (int j = 0; j < QTQ_GRID_DIM; j++) {
	  if (m_cells[i][j] != 0) {
	    int x = i + bounds.m_bL.m_x;
	    int y = j + bounds.m_bL.m_y;
	    m_vector_quad.push_back(Point(x,y));
	  }
	}
      }
    }
        return;
    }
  
  for (int i = 0; i < QTQ_NUM_QUADS; i++) {
    if (m_quads[i] != NULL) {
      int newDim = bounds.m_dim / 2;
      int q1x = bounds.m_bL.m_x;
      int q1y = bounds.m_bL.m_y;
      int q2x = bounds.m_bL.m_x + newDim;
      int q2y = bounds.m_bL.m_y;
      int q3x = bounds.m_bL.m_x;
      int q3y = bounds.m_bL.m_y + newDim;
      int q4x = bounds.m_bL.m_x + newDim;
      int q4y = bounds.m_bL.m_y + newDim;
      
      if (i == 0) {
	BBox newBounds = BBox(Point(q1x,q1y), newDim);
	m_quads[i] -> treverseTree(newBounds);
      }
      if (i == 1) {
	BBox newBounds = BBox(Point(q2x,q2y), newDim);
	m_quads[i] -> treverseTree(newBounds);
        
      }
      if (i == 2) {
	BBox newBounds = BBox(Point(q3x,q3y), newDim);
	m_quads[i] -> treverseTree(newBounds);
        
      }
      if (i == 3) {
	BBox newBounds = BBox(Point(q4x,q4y), newDim);
	m_quads[i] -> treverseTree(newBounds);
      }
    }
  }
  
}

void QTQuad::pruning(const BBox &bounds){
  if (bounds.m_dim == QTQ_GRID_DIM) {
    if (m_cells != NULL) {
      delete [] m_cells;
      m_cells = NULL;
    }
    return;
  }
  
  if (bounds.m_dim != QTQ_GRID_DIM ) {
    for (int i = 0; i < QTQ_NUM_QUADS; i++) {
      int newDim = bounds.m_dim / 2;
      int q1x = bounds.m_bL.m_x;
      int q1y = bounds.m_bL.m_y;
      int q2x = bounds.m_bL.m_x + newDim;
      int q2y = bounds.m_bL.m_y;
      int q3x = bounds.m_bL.m_x;
      int q3y = bounds.m_bL.m_y + newDim;
      int q4x = bounds.m_bL.m_x + newDim;
      int q4y = bounds.m_bL.m_y + newDim;
      
      if (i == 0) {
	if (m_quads[i] != NULL) {
	  BBox newBounds = BBox(Point(q1x,q1y), newDim);
	  m_quads[i] -> pruning(newBounds);
	  delete m_quads[i];
	  m_quads[i] = NULL;
	}
      }
      if (i == 1) {
	if (m_quads[i] != NULL) {
	  BBox newBounds = BBox(Point(q2x,q2y), newDim);
	  m_quads[i] -> pruning(newBounds);
	  delete m_quads[i];
	  m_quads[i] = NULL;
	}
        
      }
      if (i == 2) {
	if (m_quads[i] != NULL) {
	  BBox newBounds = BBox(Point(q3x,q3y), newDim);
	  m_quads[i] -> pruning(newBounds);
	  delete m_quads[i];
	  m_quads[i] = NULL;
	}
        
      }
      if (i == 3) {
	if (m_quads[i] != NULL) {
	  BBox newBounds = BBox(Point(q4x,q4y), newDim);
	  m_quads[i] -> pruning(newBounds);
	  delete m_quads[i];
	  m_quads[i] = NULL;
	}
      }
      
    }
  }
}
