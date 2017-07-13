/*********************************************************************
 * This is a model of the emission as a function of distance from    *
 * the magnetopause at the sub-solar point in the MHD active model.  *
 * The emission is only as a function of X and starts at X=03.       *
 *                                                                   *
 * This model can be used together with a magnetopause model,        *
 * wrapping this model around the magnetopause model by passing to   *
 * this model the distance from the magnetopause.                    *
 *********************************************************************/


class MODELMHDSUBSOLAR: public MODEL{
public:
  MODELMHDSUBSOLAR();
  ~MODELMHDSUBSOLAR();
  virtual double brightness(const aVec &p) const;
  virtual double losStart(const aVec &s, const aVec &d) const;
  virtual double losStop(const aVec &s, const aVec &d) const;
  virtual double losStep(const aVec &s, const aVec &d) const;
  virtual double los(const aVec &s, const aVec &d) const;  
private:
  void initialize();
  
  double x1;
  double x2;
  double x3;
  double a1;
  double a2;
  double b2;
};
