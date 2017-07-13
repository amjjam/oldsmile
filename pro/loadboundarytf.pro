function loadBoundaryTF,file,format=format

;+
; NAME:
;   loadBoundaryTF
; CALLING SEQUENCE:
;   d=loadBoundaryTF(file)
; INPUTS:
;   FILE=STRING. File containing the data
; KEYWORDS:
;   FORMAT=LONG. Format of the file. If format=0. Then x, y, z
;   preceeds r. If format=1 then the file just contains r.
; OUTPUTS:
;   Structure containing data from file returned.
;-

  if not(keyword_set(format)) then format=0

  openr,un,file,/get_lun
  nt=0l & np=0l
  readf,un,nt,np
  t=dblarr(nt)
  readf,un,t
  p=dblarr(np)
  readf,un,p

  r={t:t,p:p}

  if format eq 0 then begin
     x=dblarr(nt,np)
     y=dblarr(nt,np)
     z=dblarr(nt,np)
     readf,un,x,y,z
     r=create_struct(r,'x',x)
     r=create_struct(r,'y',y)
     r=create_struct(r,'z',z)
  endif

  rr=dblarr(nt,np)
  readf,un,rr
  r=create_struct(r,'r',rr)
  
  free_lun,un

  return,r
end
