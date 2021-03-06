/*
*  File:        ptree.cpp
*  Description: Implementation of a partitioning tree class for CPSC 221 PA3
*  Date:        2022-03-03 01:53
*
*               ADD YOUR PRIVATE FUNCTION IMPLEMENTATIONS TO THE BOTTOM OF THIS FILE
*/

#include "ptree.h"
#include "hue_utils.h" // useful functions for calculating hue averages
#include <assert.h>

using namespace cs221util;
using namespace std;

// The following definition may be convenient, but is not necessary to use
typedef pair<unsigned int, unsigned int> pairUI;

/////////////////////////////////
// PTree private member functions
/////////////////////////////////

/*
*  Destroys all dynamically allocated memory associated with the current PTree object.
*  You may want to add a recursive helper function for this!
*  POST: all nodes allocated into the heap have been released.
*/
void PTree::Clear() {
  KILL(root);
}

void PTree::KILL(Node* node) {
  if(node) {
    KILL(node->A);
    KILL(node->B);
    delete node;
  }
}

/*
*  Copies the parameter other PTree into the current PTree.
*  Does not free any memory. Should be called by copy constructor and operator=.
*  You may want a recursive helper function for this!
*  PARAM: other - the PTree which will be copied
*  PRE:   There is no dynamic memory associated with this PTree.
*  POST:  This PTree is a physically separate copy of the other PTree.
*/
void PTree::Copy(const PTree& other) {
  // add your implementation below
  
}

/*
*  Private helper function for the constructor. Recursively builds the tree
*  according to the specification of the constructor.
*  You *may* change this if you like, but we have provided here what we
*  believe will be sufficient to use as-is.
*  PARAM:  im - full reference image used for construction
*  PARAM:  ul - upper-left image coordinate of the currently building Node's image region
*  PARAM:  w - width of the currently building Node's image region
*  PARAM:  h - height of the currently building Node's image region
*  RETURN: pointer to the fully constructed Node
*/
Node* PTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  if(h == 0 || w == 0) {
    return nullptr;
  }
  if(h == 1 && w == 1) {
    Node* leaf = new Node();
    leaf->upperleft = ul;
    leaf->width = 1;
    leaf->height = 1;
    leaf->A = nullptr;
    leaf->B = nullptr;
    leaf->avg = *(im.getPixel(ul.first, ul.second));
    //cout<<"leaf found for ("<<ul.first<<", " <<ul.second<<")"<<endl;
    //cout<<"leaf avg: " << leaf->avg<<endl;
    return leaf;
  }
  if(w > h || w == h) {
    Node* cur_node = new Node();
    //vertical split
    unsigned int a_width = w / 2;
    unsigned int b_width = w - a_width;
    cur_node->upperleft = ul;
    cur_node->width = w;
    cur_node->height = h;
    

    cur_node->A = BuildNode(im, {ul.first, ul.second}, a_width, h);
    cur_node->B = BuildNode(im, {ul.first + a_width, ul.second}, b_width, h);
    // HSLAPixel* lp = im.getPixel(ul.first, ul.second);
    // HSLAPixel* rp = im.getPixel(ul.first + a_width, ul.second);
    HSLAPixel* lp = &(cur_node->A->avg);
    HSLAPixel* rp = &(cur_node->B->avg);
    assert(lp->h < 360 && lp-> h >= 0);
    assert(rp->h < 360 && rp-> h >= 0);
    //average children
    double hue = findAverage(im, {ul.first, ul.second}, w, h);


    // double lx = Deg2X(lp->h);
    // double rx = Deg2X(rp->h);
    // double ly = Deg2Y(lp->h);
    // double ry = Deg2Y(rp->h);
    // double avgx = a_width * lx/w + b_width * rx/w;
    // double avgy = a_width * ly/w + b_width * ry/w;
    // double avgx2 = -1 * avgx;
    // double avgy2 = -1 * avgy;
    // cout<<"--------------------------------"<<endl;
    // cout<<lp->h <<" and " << rp->h<<endl;
    // cout<<"(" << a_width <<" * " << lx << " / " << w << ") + (" << b_width<<" * " << rx << " / " << w<< ") = " << avgx <<endl;
    // cout<<"(" << a_width <<" * " << ly << " / " << w << ") + (" << b_width<<" * " << ry << " / " << w<< ") = " << avgy << endl;
    // cout<<"final: " << XY2Deg(avgx, avgy)<<endl;
    // cout<<"--------------------------------"<<endl;
    // double hue = XY2Deg(avgx, avgy);
    // double hue2 = XY2Deg(avgx2, avgy2);
    HSLAPixel pix;
    pix.h = hue;



    pix.s = lp->s/w + rp->s/w;
    pix.l = lp->l/w + rp->l/w;
    pix.a = lp->a/w + rp->a/w;
    cur_node->avg = pix;
    //cout<<"L EXAMPLE: "<< lp->l <<" / "<< a_width<< " + " << rp->l <<" / " << b_width << " = " << pix.l << endl;
    //cout<<"ul: ("<<ul.first<<", " << ul.second<<") w: " << w << ", h: " << h <<" || avg: " << cur_node->avg<<endl;
    return cur_node;
  }
  else if (h > w) {
    //horizontal split
    Node* cur_node = new Node();
    unsigned int a_height = h / 2;
    unsigned int b_height = h - a_height;
    cur_node->upperleft = ul;
    cur_node->width = w;
    cur_node->height = h;
    
    

    cur_node->A = BuildNode(im, {ul.first, ul.second}, w, a_height);
    cur_node->B = BuildNode(im,  {ul.first, ul.second + a_height}, w, b_height);



    // HSLAPixel* up = im.getPixel(ul.first, ul.second);
    // HSLAPixel* dp = im.getPixel(ul.first, ul.second + a_height);
    HSLAPixel* up = &(cur_node->A->avg);
    HSLAPixel* dp = &(cur_node->B->avg);
    // assert(dp->h < 360 && dp-> h >= 0);
    // assert(up->h < 360 && up-> h >= 0);
    // double ux = Deg2X(up->h);
    // double dx = Deg2X(dp->h);
    // double uy = Deg2Y(up->h);
    // double dy = Deg2Y(dp->h);
    // // double avgx = ux/a_height + dx/b_height;
    // // double avgy = uy/a_height + dy/b_height;
    // double avgx = a_height * ux/h + b_height * dx/h;
    // double avgy = a_height * uy/h + b_height * dy/h;
    // double avgx2 = -1 * avgx;
    // double avgy2 = -1 * avgy;
    // double hue = XY2Deg(avgx, avgy);
    // double hue2 = XY2Deg(avgx2, avgy2);
    double hue = findAverage(im, {ul.first, ul.second}, w, h);
    HSLAPixel pix;
    pix.h = hue;
    // cout<<"--------------------------------"<<endl;
    // cout<<up->h <<" and " << dp->h<<endl;
    // cout<<"(" << a_height <<" * " << ux << " / " << h << ") + (" << b_height<<" * " << dx << " / " << h<< ") = " << avgx <<endl;
    // cout<<"(" << a_height <<" * " << uy << " / " << h << ") + (" << b_height<<" * " << dy << " / " << h<< ") = " << avgy << endl;
    // cout<<"final: " << pix.h <<endl;
    // cout<<"--------------------------------"<<endl;
    pix.s = up->s/h + dp->s/h;
    pix.l = up->l/h + dp->l/h;
    pix.a = up->a/h + dp->a/h;
    cur_node->avg = pix;
    //cout<<"ul: ("<<ul.first<<", " << ul.second<<") w: " << w << ", h: " << h <<" || avg: " << cur_node->avg<<endl;
    return cur_node;
  }
}

double PTree::findAverage(PNG im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  double x_comp = 0;
  double y_comp = 0;
  //cout<<"----------- w: " << w << ", h: " << h << "       and starts ("<<ul.first<<", " <<ul.second<<")"<<endl;
  for(unsigned int i = 0; i < w; i++) {
    for(unsigned int j = 0; j < h; j++) {
      //cout<<"adding x: " << Deg2X(im.getPixel(ul.first + i, ul.second + j)->h) << "        adding y: " << Deg2Y(im.getPixel(ul.first + i, ul.second + j)->h) << endl;
      x_comp += Deg2X(im.getPixel(ul.first + i, ul.second + j)->h);
      y_comp += Deg2Y(im.getPixel(ul.first + i, ul.second + j)->h);
    }
  }
  double xavg = x_comp / (w*h);
  double yavg = y_comp / (w*h);
  double hue = XY2Deg(xavg, yavg);
  double hue2 = XY2Deg(-1 * xavg, -1 * yavg);
  //cout<<"xavg: " << xavg<< ", yavg: "<<yavg<<endl;
  //cout<<"return "<< min(hue, hue2) << endl;;
  return hue;
}



////////////////////////////////
// PTree public member functions
////////////////////////////////

/*
*  Constructor that builds the PTree using the provided PNG.
*
*  The PTree represents the sub-image (actually the entire image) from (0,0) to (w-1, h-1) where
*  w-1 and h-1 are the largest valid image coordinates of the original PNG.
*  Each node corresponds to a rectangle of pixels in the original PNG, represented by
*  an (x,y) pair for the upper-left corner of the rectangle, and two unsigned integers for the
*  number of pixels on the width and height dimensions of the rectangular sub-image region the
*  node defines.
*
*  A node's two children correspond to a partition of the node's rectangular region into two
*  equal (or approximately equal) size regions which are either tiled horizontally or vertically.
*
*  If the rectangular region of a node is taller than it is wide, then its two children will divide
*  the region into vertically-tiled sub-regions of equal height:
*  +-------+
*  |   A   |
*  |       |
*  +-------+
*  |   B   |
*  |       |
*  +-------+
*
*  If the rectangular region of a node is wider than it is tall, OR if the region is exactly square,
*  then its two children will divide the region into horizontally-tiled sub-regions of equal width:
*  +-------+-------+
*  |   A   |   B   |
*  |       |       |
*  +-------+-------+
*
*  If any region cannot be divided exactly evenly (e.g. a horizontal division of odd width), then
*  child B will receive the larger half of the two subregions.
*
*  When the tree is fully constructed, each leaf corresponds to a single pixel in the PNG image.
* 
*  For the average colour, this MUST be computed separately over the node's rectangular region.
*  Do NOT simply compute this as a weighted average of the children's averages.
*  The functions defined in hue_utils.h and implemented in hue_utils.cpp will be very useful.
*  Computing the average over many overlapping rectangular regions sounds like it will be
*  inefficient, but as an exercise in theory, think about the asymptotic upper bound on the
*  number of times any given pixel is included in an average calculation.
* 
*  PARAM: im - reference image which will provide pixel data for the constructed tree's leaves
*  POST:  The newly constructed tree contains the PNG's pixel data in each leaf node.
*/
PTree::PTree(PNG& im) {
  root = BuildNode(im, {0, 0}, im.width(), im.height());
  cout<<"root avg "<< root->avg<<endl;
}

/*
*  Copy constructor
*  Builds a new tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  This tree is constructed as a physically separate copy of other tree.
*/
PTree::PTree(const PTree& other) {
  root = CopyNode(other.root);
}

Node* PTree::CopyNode(Node* node) {
  if(node == nullptr) {
    return nullptr;
  }
  Node* cur = new Node();
  cur->height = node->height;
  cur->width = node->width;
  cur->upperleft = node->upperleft;
  cur->A = CopyNode(node->A);
  cur->B = CopyNode(node->B);
  return cur;
}

/*
*  Assignment operator
*  Rebuilds this tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  If other is a physically different tree in memory, all pre-existing dynamic
*           memory in this tree is deallocated and this tree is reconstructed as a
*           physically separate copy of other tree.
*         Otherwise, there is no change to this tree.
*/
PTree& PTree::operator=(const PTree& other) {
  if(this->root == other.root)
    return *this;

  //implement deallocating old memory
  PTree* copied = new PTree(other);
  
  Clear();
  return *copied;
}

/*
*  Destructor
*  Deallocates all dynamic memory associated with the tree and destroys this PTree object.
*/
PTree::~PTree() {
  //KILL(root);
}

/*
*  Traverses the tree and puts the leaf nodes' color data into the nodes'
*  defined image regions on the output PNG.
*  For non-pruned trees, each leaf node corresponds to a single pixel that will be coloured.
*  For pruned trees, each leaf node may cover a larger rectangular region that will be
*  entirely coloured using the node's average colour attribute.
*
*  You may want to add a recursive helper function for this!
*
*  RETURN: A PNG image of appropriate dimensions and coloured using the tree's leaf node colour data
*/
PNG PTree::Render() const {
  if(root == nullptr) {
    return PNG();
  }
  PNG png;
  //cout<<"presize"<<endl;
  png.resize(root->width, root->height);
  //cout<<"png resized: " << png->width() << ", " << png->height() << endl;
  //cout<<"STAAAAAAAAAAAARTING RENDERRRRRRRRRRRRRRRRRRRRR"<<endl;
  RenderHelper(root, png);
  //cout<<"FINISHED RENDER"<<endl;
  //cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
  //cout<<*(png.getPixel(0,2))<<endl;
  return png;
}

Node* PTree::RenderHelper(Node* node, PNG& png) const {
  if(node == nullptr) {
    return nullptr;
  }
  if(node->width == 1 && node->height == 1) {
    HSLAPixel* pix = png.getPixel(node->upperleft.first, node->upperleft.second);
    //cout<<"pixel: " << *pix <<endl;
    *pix = (node->avg);
    //cout<<"newPixel: " << *pix << endl;
  }
  if(node->A != nullptr)
    node->A = RenderHelper(node->A, png);
  if(node->B != nullptr)
    node->B = RenderHelper(node->B, png);
  return node;
}

/*
*  Trims subtrees as high as possible in the tree. A subtree is pruned
*  (its children are cleared/deallocated) if ALL of its leaves have colour
*  within tolerance of the subtree root's average colour.
*  Pruning criteria should be evaluated on the original tree, and never on a pruned
*  tree (i.e. we expect that Prune would be called on any tree at most once).
*  When processing a subtree, you should determine if the subtree should be pruned,
*  and prune it if possible before determining if it has subtrees that can be pruned.
* 
*  You may want to add (a) recursive helper function(s) for this!
*
*  PRE:  This tree has not been previously pruned (and is not copied/assigned from a tree that has been pruned)
*  POST: Any subtrees (as close to the root as possible) whose leaves all have colour
*        within tolerance from the subtree's root colour will have their children deallocated;
*        Each pruned subtree's root becomes a leaf node.
*/
void PTree::Prune(double tolerance) {
  // add your implementation below
  
}

/*
*  Returns the total number of nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::Size() const {
  return TreeSize(root);
}

int PTree::TreeSize(const Node* node) const {
  if(node == nullptr) 
    return 0;
  return 1 + TreeSize(node->A) + TreeSize(node->B);
}

/*
*  Returns the total number of leaf nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::NumLeaves() const {
  // replace the line below with your implementation
  return leafCount(root);
}

int PTree::leafCount(const Node* node) const {
  if (node == nullptr) {
    return 0;
  }
  if (node->A == nullptr && node->B == nullptr) {
    return 1;
  }
  
  return leafCount(node->A) + leafCount(node->B);
}

/*
*  Rearranges the nodes in the tree, such that a rendered PNG will be flipped horizontally
*  (i.e. mirrored over a vertical axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped horizontally.
*/
void PTree::FlipHorizontal() {
  // add your implementation below
  mirror(root);
}

void PTree::mirror(Node* subRoot)
{
    // Base case
    if (subRoot == NULL) {
      return;
    }

    std::swap(subRoot->A, subRoot->B);
    mirror(subRoot->A);
    mirror(subRoot->B);
}

/*
*  Like the function above, rearranges the nodes in the tree, such that a rendered PNG
*  will be flipped vertically (i.e. mirrored over a horizontal axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped vertically.
*/
void PTree::FlipVertical() {
  // add your implementation below
  mirror(root);
}

/*
    *  Provides access to the root of the tree.
    *  Dangerous in practice! This is only used for testing.
    */
Node* PTree::GetRoot() {
  return root;
}

//////////////////////////////////////////////
// PERSONALLY DEFINED PRIVATE MEMBER FUNCTIONS
//////////////////////////////////////////////

