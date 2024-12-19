      subroutine  EvaluateScore1(w,T,p,h)

!Function: Evaluation of sample score in one point
!Purpose:  Developed in windows fortran, portable to linux fortran.
!          Main purpose is to implement this function efficiently.
!
!Author: Costas Florios, 27.10.2006
      integer Tmax,pmax
      parameter (Tmax=50000, pmax=20)
      double precision y(Tmax), Z(pmax,Tmax),b0
      integer data_y(Tmax)
!arguments
      double precision w(pmax)
      integer T,p
      double precision h
!locals
      double precision beta(pmax)
      double precision sump(Tmax)
      integer firep(Tmax),sfire
      double precision hfire

      common/main1/ y,Z,b0,data_y

!initializations
      hfire=0.
      do i=1,T
         sump(i)=0.
         firep(i)=0
      enddo

      do i=1,p
         beta(i)=w(i)
      enddo

      sfire=0        
      do i=1,T
         sump(i)=0
         do j=1,p
    
!sump(i)=sump(i)+beta(j)*Z(j,i)
            sump(i)=sump(i)+beta(j)*Z(j+1,i)
         enddo
!sump(i)=sump(i)+b0
         sump(i)=sump(i)+b0*Z(1,i)
         if (sump(i).ge.0) then
             firep(i)=1
         else
             firep(i)=-1
         endif
         sfire=sfire+data_y(i)*firep(i)
      enddo
!sfire is score in -1 1 range
!hfire is score in 0 1 range
!EvaluateScore1=sfire
!     write(*,*) sfire,dfloat(sfire)
      hfire=dfloat(sfire)/dfloat(T)
!     write(*,*) hfire
      hfire=(1.+hfire)/2.
!     write(*,*) hfire
      h=hfire
!now comparison to matlab is straightforward
!done
      return
      end subroutine
