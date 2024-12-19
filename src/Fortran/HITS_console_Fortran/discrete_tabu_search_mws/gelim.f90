subroutine gelim(np,sk,r1,u)
integer Tmax,pmax
parameter (Tmax=50000, pmax=20)
!common/gauss1/np,sk(nnmax,nnmax),r1(nnmax),u(nnmax)

integer np
double precision sk(pmax,pmax),r1(pmax),u(pmax)

!locals
double precision bsk(pmax,pmax),br1(pmax)

!np=p
!sk=A
!r1=b
!x=u

!save sk in bsk and r1 in br1 because they will be overwritten
      do i=1,np
         br1(i)=r1(i)
         do j=1,np
            bsk(i,j)=sk(i,j)
         enddo
      enddo

      do 20 i=1,np-1
      do 20 j=i+1,np
      p=sk(j,i)/sk(i,i)
      r1(j)=r1(j)-p*r1(i)
      do 20 k=1,np
      sk(j,k)=sk(j,k)-p*sk(i,k)
 20   continue
      do 30 l=np,1,-1
      u(l)=r1(l)/sk(l,l)
      do 30 j=l-1,1,-1
      p1=sk(j,l)/sk(l,l)
      sk(j,l)=0.
      r1(j)=r1(j)-p1*r1(l)
 30   continue  
 !restore sk from bsk and r1 from br1
      do i=1,np
         r1(i)=br1(i)
         do j=1,np
            sk(i,j)=bsk(i,j)
         enddo
      enddo
      return
      end
