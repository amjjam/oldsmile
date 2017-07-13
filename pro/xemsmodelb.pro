function xemsModelB,x,y,z,q,params=p

;+
; NAME:
;   xemsModelB
; PURPOSE:
;   X-ray emissions model number B. Compute model X-ray emission at a location.
; CALLING SEQUENCE:
;   r=xemsModelB(x,y,z)
; INPUTS:
;   X,Y,Z=DOUBLE. Location 
;   Q=LONG. Region
; OUTPUTS:
;   R=DOUBLE. X-ray emission.
; KEYWRDS:
;   PARAMS={A1:DOUBLE Parameters between MP and BS
;           B:DOUBLE
;           alpha:DOUBLE
;           beta:DOUBLE
;           A2:DOUBLE Parameter outside BS
;          }
;   
;-

  rref=10d
  r=sqrt(x*x+y*y+z*z)
  
  if q eq 0 then return,0

  if q eq 2 then return,p.a2*(rref/r)^3

  costheta=x/r
  sintheta=sin(acos(costheta))
  sintheta2=sintheta*sintheta
  sintheta8=sintheta2^4

  ;return,p.a1*exp(-r/rref*(p.beta+p.gamma*sintheta^4+p.delta*cos2phi))*$
  ;       (1.+p.epsilon*sintheta)

  ;return,p.a1*(r)^(-(p.beta+p.gamma*sintheta^4+p.delta*cos2phi))*$
  ;       (1.+p.epsilon*costheta)/(1.+p.epsilon)
  
  return,(p.a1+p.b*sintheta8)*(r/rref)^(-(p.alpha+p.beta*sintheta2))
end
