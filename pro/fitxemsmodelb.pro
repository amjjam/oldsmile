pro fitXemsMode1B_initialize,d,p
  common fitXemsModelBCommon,dd,pp
  ntheta=n_elements(d.theta)
  nphi=n_elements(d.phi)
  nr=(size(d.r,/dim))[0]
  dd={x:dblarr(nr,nphi,ntheta),y:dblarr(nr,nphi,ntheta),z:dblarr(nr,nphi,ntheta),q:d.q,p:d.v,theta:d.theta,phi:d.phi}
  
  for i=0l,ntheta-1 do begin
     for j=0l,nphi-1 do begin
        for k=0l,nr-1 do begin
           dd.x[k,j,i]=d.r[k,j,i]*cos(d.theta[i])
           dd.y[k,j,i]=d.r[k,j,i]*sin(d.theta[i])*cos(d.phi[j])
           dd.z[k,j,i]=d.r[k,j,i]*sin(d.theta[i])*sin(d.phi[j])
        endfor
     endfor
  endfor
  pp=p
end

function fitXemsModelB_costfunc,p
  common fitXemsModelBCommon,dd,pp
  
  pp.a1=p[0]
  pp.b=p[1]
  pp.alpha=p[2]
  pp.beta=p[3]
  pp.a2=p[4]

  diff=0d
  ntheta=n_elements(dd.theta)
  nphi=n_elements(dd.phi)
  nr=(size(dd.x,/dim))[0]
  n=0l
  for i=0l,ntheta-1 do begin
     for j=0l,nphi-1 do begin
        for k=0l,nr-1 do begin
           if dd.theta[i] lt 61./180.*!pi  then begin ;and $
;              (((dd.phi[j] gt !pi/4+0.1) and (dd.phi[j] lt 3.*!pi/4-0.1)) or $
;               ((dd.phi[j] gt 5*!pi/4+0.1) and (dd.phi[j] lt 7.*!pi/4-0.1))) then begin
              diff+=abs(dd.p[k,j,i]-xemsModelB(dd.x[k,j,i],dd.y[k,j,i],$
                                               dd.z[k,j,i],dd.q[k,j,i],$
                                               params=pp))
              n++;
           endif
        endfor
     endfor
  endfor
  
  print,n,diff/n,format='(i5,f20.16)'
  print,p

  return,diff/n
end

function fitXemsModelB,d,p,fmin

;+
; NAME:
;   fitXemsModelB
; PURPOSE:
;   Fit xemsModelB to some data
; CALLING SEQUENCE:
;   r=fitXemsModelB(d,p)
; INPUTS:
;   D={}. The data to fit. The structure returned by loadXems()
;   P={}. The PARAMS structure to xemsModelB with initial values
;   filled in.
; OUTPUTS:
;   FMIN=DOUBLE. Cost-function minimum value.
; KEYWORDS:
;-

  fitXemsMode1B_initialize,d,p
  
  pp=dblarr(5)
  pp[0]=p.a1
  pp[1]=p.b
  pp[2]=p.alpha
  pp[3]=p.beta
  pp[4]=p.a2
  xi=dblarr(5,5)
  for i=0,1 do xi[i,i]=1d-5
  for i=2,3 do xi[i,i]=.1d
  xi[4,4]=1d-5

  powell,pp,xi,1d-13,fmin,'fitXemsModelB_costfunc',/double
  
  p.a1=pp[0]
  p.b=pp[1]
  p.alpha=pp[2]
  p.beta=pp[3]
  p.a2=pp[4]

  return,p
end

@loadXemsSamples.pro
@xemsModelB.pro

d=loadXemsSamples("../adat/S0522.5V400B000005ad.xems_samples")
p={a1:3d-5,b:-1.5d-5,alpha:2.7d,beta:-2d,a2:1d-5}

r=fitXemsModelB(d,p,fmin)

;r={a1:0.0028,beta:1.94,gamma:-0.13,delta:0,epsilon:1.31,a2:1.32e-5}

;; x=dblarr(
;; x=d.r*cos(d.theta)
;; y=d.r*sin(d.theta)*cos(d.phi)
;; z=d.r*sin(d.theta)*sin(d.phi)

m=d

p=r

nr=(size(d.r,/dim))[0]
for i=0,n_elements(d.theta)-1 do begin
   for j=0,n_elements(d.phi)-1 do begin
      for k=0,nr-1 do begin
         m.v[k,j,i]=xemsModelB(d.r[k,j,i]*cos(d.theta[i]),$
                               d.r[k,j,i]*sin(d.theta[i])*cos(d.phi[j]),$
                               d.r[k,j,i]*sin(d.theta[i])*sin(d.phi[j]),$
                               d.q[k,j,i],$
                               params=p)
      endfor
   endfor
endfor

;; for i=0,n_elements(x)-1 do begin
;;    m.v[i]=xemsModelB(x[i],y[i],z[i],d.q[i],params=p)   
;; endfor


save,filename='../adat/fitXemsModelB.idl',p,d,m

end
