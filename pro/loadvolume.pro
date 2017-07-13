function loadVolume,file

;+
; NAME:
;    loadVolume
; PURPOSE:
;    Loads a volume 
; CALLING SEQUENCE:
;    d=loadVolume(file)
; INPUTS:
;    FILE=STRING. Name of file to load
; OUTPUTS:
;    D={xmin,xmax,ymin,ymax,zmin,zmax=DOUBLE
;       nx,ny,nz=LONG
;       DATA=DBLARR(nx,ny,nz)
;      }
;-

openr,un,file,/get_lun,/compress

xmin=0d & xmax=0d & nx=0l & ymin=0d & ymax=0d & ny=0l
zmin=0d & zmax=0d & nz=0l

readu,un,xmin,xmax,nx,ymin,ymax,ny,zmin,zmax,nz

data=dblarr(nz,ny,nx)

readu,un,data

free_lun,un

return,{xmin:xmin,xmax:xmax,nx:nx,ymin:ymin,ymax:ymax,ny:ny,$
        zmin:zmin,zmax:zmax,nz:nz,data:transpose(data)}
end
