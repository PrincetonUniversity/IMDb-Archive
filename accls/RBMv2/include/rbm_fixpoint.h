/// \file the ila example of RBM
///  Hongce Zhang (hongcez@princeton.edu)
///

#ifndef RBM_FIXPOINT_H__
#define RBM_FIXPOINT_H__

#include <ilang/ilang++.h>

using namespace ilang;

struct FixpointFormat {
  public:
    /// the total width of the fixedpoint number
    unsigned w;
    /// the integer width of the fixedpoint number
    unsigned i;
    /// True if it is signed (has a sign bit)
    bool sign;
    /// The constructor
    FixpointFormat (unsigned _w, unsigned _i, bool _s = false) :
      w(_w), i(_i), sign(_s) {}
}; // end of FixpointFormat

class Fixpoint {
  public:
    /// Constructor: ast is the raw bits and format is the format
    Fixpoint(ExprRef ast, const FixpointFormat & format);
    //
    FixpointFormat getFormat() const;
    Fixpoint asSigned() const;
    Fixpoint asUnsigned() const;
    ExprRef astToFormat() const;
    ExprRef get() const;

    friend Fixpoint operator+(const Fixpoint &l, const Fixpoint &r);
    friend Fixpoint operator-(const Fixpoint &l, const Fixpoint &r);
    friend Fixpoint operator*(const Fixpoint &l, const Fixpoint &r);
    friend bool operator==(const Fixpoint &l, const Fixpoint &r);
    friend bool operator!=(const Fixpoint &l, const Fixpoint &r);
    friend bool operator<(const Fixpoint &l, const Fixpoint &r);
    friend bool operator<=(const Fixpoint &l, const Fixpoint &r);
    friend bool operator>(const Fixpoint &l, const Fixpoint &r);
    friend bool operator>=(const Fixpoint &l, const Fixpoint &r);


  protected:
    /// The raw bits of the fixedpoint number
    ExprRef ast;
    /// the total width of the fixedpoint number
    unsigned w;
    /// the integer width of the fixedpoint number
    unsigned i;
    /// True if it is signed (has a sign bit)
    bool sign;


}; // end of Fixpoint


Fixpoint fpconst(double v, const FixpointFormat & format);
Fixpoint fpconvert(ExprRef ast, const FixpointFormat & from, const FixpointFormat & to);


#endif // RBM_FIXPOINT_H__
