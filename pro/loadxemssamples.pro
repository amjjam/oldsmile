function loadXemsSamples,file

;+
; NAME:
;   loadXemsSamples
; PURPOSE:
;   Load a file which was written by the sampleXems
; CALLING SEQUENCE:
;   r=loadXems(file)
; INPUTS:
;   FILE=STRING. Name of samples file to load.
; OUTPUTS:
;   D={THETA:DBLARR(NTHETA) theta coordinates
;      PHI:DBLARR(NPHI) phi coordinates
;      R:DBLARR(NR,NPHI,NTHETA) r coordinates
;      V:DBLARR(NR,NPHI,NTHETA)
;      Q:LONARR(NR,NPHI,NTHETA) 0: inside mp, 1: inside bs, 2: outside bs
;     }
;+

  openr,un,file,/get_lun
  ntheta=0l & nphi=0l & nr=0l
  readu,un,ntheta,nphi,nr
  
  theta=dblarr(ntheta)
  readu,un,theta

  phi=dblarr(nphi)
  readu,un,phi

  r=dblarr(nr,nphi,ntheta)
  v=dblarr(nr,nphi,ntheta)
  q=lonarr(nr,nphi,ntheta)

  tmpr=0d
  tmpv=0d
  tmpq=0l

  for i=0,ntheta-1 do begin
     for j=0,nphi-1 do begin
        for k=0,nr-1 do begin
           readu,un,tmpr,tmpv,tmpq
           r[k,j,i]=tmpr
           v[k,j,i]=tmpv
           q[k,j,i]=tmpq
        endfor
     endfor
  endfor
  
  free_lun,un
  
  return,{theta:theta,phi:phi,r:r,v:v,q:q}
end
