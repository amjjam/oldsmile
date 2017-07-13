;-
;
; Do LADFIT on V as a function of R for every theta and phi value. Fit
; two functions:
;
; V=A1*(R/Rref)^(-B1)
;
; linear version: alog(V)=alog(A1)-B1*alog(R/Rref)
;
; and 
; V=A2*exp(-B2*(R/Rref))
;
; linear version: alog(V)=alog(A2)-B2*(R/Rref)
;
; Also try several different values for RRef: 1, 2, 5, 10, 20 Re
;
;-

d=loadXemsSamples("../adat/S0522.5V400B000005ad_dense.xems_samples")

rref=[1d,2,5,10,20]
ntheta=n_elements(d.theta)
nphi=n_elements(d.phi)
nrref=n_elements(rref)
a1=dblarr(nphi,ntheta,nrref)
b1=a1
d1=a1
a2=a1
b2=a1
d2=a1

for irref=0l,nrref-1 do begin
   print,irref,nrref
   for itheta=0l,ntheta-1 do begin
      for iphi=0l,nphi-1 do begin
         index=where(d.q[*,iphi,itheta] eq 1)
         result=ladfit(alog(d.r[index,iphi,itheta]/rref[irref]),$
                       alog(d.v[index,iphi,itheta]),$
                       absdev=tmp)
         a1[iphi,itheta,irref]=exp(result[0])
         b1[iphi,itheta,irref]=-result[1]
         d1[iphi,itheta,irref]=tmp;
         result=ladfit(d.r[index,iphi,itheta]/rref[irref],$
                       alog(d.v[index,iphi,itheta]),$
                       absdev=tmp)
         a2[iphi,itheta,irref]=exp(result[0])
         b2[iphi,itheta,irref]=-result[1]
         d2[iphi,itheta,irref]=tmp
      endfor
   endfor
endfor

d={rref:rref,theta:d.theta,phi:d.phi,a1:a1,b1:b1,d1:d1,a2:a2,b2:b2,d2:d2}

save,filename="../adat/fitXemsSlopes.idl",d

end
