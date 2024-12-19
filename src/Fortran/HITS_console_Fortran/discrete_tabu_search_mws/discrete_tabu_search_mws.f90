      program discrete_tabu_search_mws

!Program: Implementation of Discrete Tabu Search algorithm for 
!         maximum score estimation based on Discrete Local Search 
!         which is further based on Pinkse93 exact algorithm article
!         in Journal of Econometrics.
!Purpose: Developed in windows fortran, portable to linux fortran.
!         Main purpose is to seehow good this tabu search
!         algorithm can be and compare to local search/simulated annealing
!         already developed algorithms.
!         Applied to Horowitz dataset, minor changes
!
!Author: Costas Florios, 25.10.2011

      integer Tmax,pmax,Itmax
      parameter (Tmax=50000, pmax=20, Itmax=1000)
      double precision y(Tmax), Z(pmax,Tmax),b0
      integer data_y(Tmax)
      integer id(Tmax)
!locals
      double precision A(pmax,pmax),b(pmax),bb(pmax)
      integer i,j,l,ii,jj,ll,mii,mjj,mll,dii,djj,dll
      integer n,k
      double precision mA(pmax,pmax),mb(pmax),bmb(pmax)
      double precision x(pmax),score1,MaxScore0,MaxScore,MaxScoreEver,temp
      integer p,T
      integer row
      integer dummy,dummy1,dummy2
      double precision Af(pmax,pmax),bf(pmax),bbf(pmax)
      integer mqq,qq,q
      double precision ArgMax(pmax),MaxScoreF,MaxScoreFF
        
      real time_begin,time_middle,time_end
      
      integer nat(pmax),natsol(pmax),natcur(pmax),natever(pmax)
      
      logical anstrivial
      
      double precision xlbound(pmax),xubound(pmax)
      
      integer Mat(pmax,Itmax),snat(pmax),snatsol(pmax),snatcur(pmax)
      
      logical anstabu
      
      data ntenure,niterover,niternotimproved/50,200,50/
        
      common/main1/ y,Z,b0,data_y
!     common/gauss1/
              
!input files
      open(unit=1,file='Horowitz93fortran_y.txt',status='unknown')
      open(unit=2,file='Horowitz93fortran_z_intcpt.txt',status='unknown')
!output files
      open(unit=3,file='Horowitz93fortran_tabusearch_restart1_opt_bounds_1e4_res_iter200.out',status='unknown')
      open(unit=4,file='Horowitz93fortran_tabusearch_restart1_opt_bounds_1e4_info_iter200.out',status='unknown')
      open(unit=5,file='Horowitz93fortran_tabusearch_restart1_opt_bounds_1e4_log_iter200.out',status='unknown')
      

      write(*,*) 'this is Discrete tabu search algorithm'
      write(*,*) 'for maximum score estimation'
      write(3,*) 'this is Discrete tabu search algorithm'
      write(3,*) 'for maximum score estimation'
      write(4,*) 'this is Discrete tabu search algorithm'
      write(4,*) 'for maximum score estimation'
      write(5,*) 'this is Discrete tabu search algorithm'
      write(5,*) 'for maximum score estimation'

      write(*,*) 'Give dimension of sample T and number of parameters p'
      write(*,*) '(intercept is indeed counted in p due to normalization)'

      read(*,*) T,p

!read y and Z
      pp=p+1
!read y
      do i=1,T
         read(1,*) id(i),data_y(i)
      enddo
      read(2,*) ((Z(j,i),j=1,pp),i=1,T)
!read Z
      do j=2,pp+1
         do i=1,T
            Z(j-1,i)=Z(j,i)
         enddo
      enddo
!read b0
      write(*,*) 'Give case for coefficient of variable 0, b(0): 1 or -1'
      read(*,*) b0

!compute y as data_y but as a double (trivial, y not used anywhere)
      do i=1,T
          y(i)=data_y(i)
      enddo

!input of data_y,Z,b0 and definition of y is ok!

call cpu_time(time_begin)
!Start process...
!probit initial solution for clogit_aironly dataset
!mii=48
!mjj=100
!mll=160
!naive initial solution for clogit_aironly_dataset

mii=83
mjj=599
mll=827
mqq=100
nat(1)=mii
nat(2)=mjj
nat(3)=mll
nat(4)=mqq


do j=1,p-1
    xlbound(j)=-1D4
    xubound(j)=+1D4
enddo
!now on, extract intercept from number of parameters
p=p-1

call bsort(p,nat,snat)

do j=1,p
    nat(j)=snat(j)
enddo

do j=1,p
    natsol(j)=nat(j)
enddo
!mqq=4
do j=1,p
    if (y(id(nat(j))).ge.0) then
        mb(j)=1
    else
        mb(j)=0
    endif
enddo    
!compute mA
do j=1,p
    do i=1,p
        mA(i,j)=(1-2*mb(i))*Z(j+1,id(nat(i)))
!        mA(2,j)=(1-2*mb(2))*Z(j,id(nat(2)))
!        mA(3,j)=(1-2*mb(3))*Z(j,id(nat(3)))
    enddo
enddo
!don't overwrite mb, define bmb
do i=1,p
    bmb(i)=b0*(-1+2*mb(i))*Z(1,id(nat(i)))
enddo

call gelim(p,mA,bmb,x)

do i=1,p
    if (x(i).lt.xlbound(i).OR.x(i).gt.xubound(i)) then
        write(*,*) 'Initial Solution ',(x(iii),iii=1,p),' is out of bounds'    
        write(*,*) 'Increase bounds or try another initial solution'
        write(3,*) 'Initial Solution ',(x(iii),iii=1,p),' is out of bounds'    
        write(3,*) 'Increase bounds or try another initial solution'
        write(4,*) 'Initial Solution ',(x(iii),iii=1,p),' is out of bounds'    
        write(4,*) 'Increase bounds or try another initial solution'
        write(5,*) 'Initial Solution ',(x(iii),iii=1,p),' is out of bounds'    
        write(5,*) 'Increase bounds or try another initial solution'
        stop
    endif
enddo

call EvaluateScore1(x,T,p,score1)
MaxScore0=score1
ii=mii
jj=mjj
ll=mll
MaxScore=MaxScore0
MaxScoreEver=MaxScore0
write(*,*)'Initial score is ',MaxScore0
write(3,*)'Initial score is ',MaxScore0
write(4,*)'Initial score is ',MaxScore0
write(5,*)'Initial score is ',MaxScore0
write(*,*)'Initial attributes ',(nat(j),j=1,p)
write(3,*)'Initial attributes ',(nat(j),j=1,p)
write(4,*)'Initial attributes ',(nat(j),j=1,p)
write(5,*)'Initial attributes ',(nat(j),j=1,p)

do j=1,p
    do i=1,ntenure
       Mat(j,i)=-1    
    enddo
enddo
!ready to start loop of BMNH discrete tabu search for max score estimation
!Start of iterative improvement/deterioration tabusearch procedure
!Rewrite of this part and on  ... OK done!
csubstitutionstotal=0
cbdsubseq=0
999 continue	
cbd=0
cinfsd=0
dz=-1
dzmax=-1
n=T
k=p
row=0

call bsort(p,natsol,snatsol)

do i=1,p
    natsol(i)=snatsol(i)
enddo
!compute b
do i=1,p
    if (y(id(natsol(i))).ge.0) then
        b(i)=1
    else
        b(i)=0
    endif
enddo    
!compute A
do j=1,p
    do i=1,p
        A(i,j)=(1-2*b(i))*Z(j+1,id(natsol(i)))
!        A(2,j)=(1-2*b(2))*Z(j,id(natsol(2)))
!        A(3,j)=(1-2*b(3))*Z(j,id(natsol(3)))
    enddo
enddo
!don't overwrite b, define bb
do i=1,p
    bb(i)=b0*(-1+2*b(i))*Z(1,id(natsol(i)))
enddo

do i=1,p
   do j=1,n
             anstrivial=.false.
             do iii=1,p
                if (nat(iii).eq.id(j)) then
                    anstrivial=.true.
                endif              
             enddo
             !anstrivial=nat(1).eq.id(j).OR.nat(2).eq.id(j).OR.nat(3).eq.id(j)
             if ( anstrivial ) then  ! avoid examining same linear system points
                goto 100 !generate another j point which is likely to be ok
             endif
             ! insert j in pBYp linear system, remove nat(i) from i position of pBYp linear system
             row=row+1
             !nat
             do iii=1,p
                if (iii.eq.i) then
                    nat(iii)=id(j)
                else
                    nat(iii)=natsol(iii)
                endif 
             enddo
             !check tabu-active status of trial point nat through membership in Mat - ntenure - last columns
             call bsort(p,nat,snat)
             !if (csubstitutionstotal.eq.2.AND.nat(1).eq.20.AND.nat(2).eq.100.AND.nat(3).eq.108) then
             !    continue
             !endif
             do jjj=1,ntenure
                anstabu=.false.
                ct=0
                do iii=1,p
                   if (snat(iii).eq.Mat(iii,csubstitutionstotal+jjj)) then
                        ct=ct+1
                   endif                   
                enddo
                if (ct.eq.p) then
                    anstabu=.true.
                    jjjt=jjj                    
                endif         
                if (anstabu) then
                    goto 100 !generate another j point which is likely to be ok
                endif
             enddo

             !if (anstabu.AND.csubstitutionstotal.eq.2) then
             !    goto 100 !generate another j point which is likely to be ok
             !endif            
             !Now on, it is assumed that the trial solution is not tabu-active
             !b
             do iii=1,p
                if (y(nat(iii)).ge.0) then
                    b(iii)=1
                else
                    b(iii)=0
                endif
             enddo
            !A
             do iii=1,p
                do jjj=1,p
                    A(jjj,iii)=(1-2*b(jjj))*Z(iii+1,id(nat(jjj)))
                    !A(2,iii)=(1-2*b(2))*Z(iii,id(nat(2)))
                    !A(3,iii)=(1-2*b(3))*Z(iii,id(nat(3)))
                enddo    
             enddo
             !bb
             do iii=1,p
                bb(iii)=b0*(-1+2*b(iii))*Z(1,id(nat(iii)))
             enddo
                       
             call gelim(p,A,bb,x)          
             
             do iii=1,p
                if (x(iii).lt.xlbound(iii).OR.x(iii).gt.xubound(iii)) then
                      cinfsd=cinfsd+1
                      goto 100 !generate another j point which is likely to be ok
                               !x trial point out of bounds 
                endif
             enddo             
          
             call EvaluateScore1(x,T,p,score1)
             dz=score1-MaxScore
             !benefitial swap
             if (dz.gt.0) then
                 cbd=cbd+1
                 cbdsubseq=0
             endif
             !Store Best solution ever
             if (score1.gt.MaxScoreEver) then
                 MaxScoreEver=score1
                 do iii=1,p
                    natever(iii)=nat(iii)
                 enddo
             endif
             !choose biggest dz <=>0 to achieve BMNH
             if(dz.gt.dzmax) then
		        dzmax=dz
		        do iii=1,p
		           natcur(iii)=nat(iii)
		        enddo
                !ii=nat(1)
                !jj=nat(2)
                !ll=nat(3)
             endif
                                   
100    continue             
       enddo
enddo

write(*,*) row,' neighbourhood moves checked for the current step'
write(3,*) row,' neighbourhood moves checked for the current step'
write(4,*) row,' neighbourhood moves checked for the current step'
write(*,*) cbd,' better domains than current solution'
write(3,*) cbd,' better domains than current solution'
write(4,*) cbd,' better domains than current solution'
write(*,*) cinfsd,' out of bounds domains rejected'
write(3,*) cinfsd,' out of bounds domains rejected'
write(4,*) cinfsd,' out of bounds domains rejected'

call cpu_time(time_middle)
csubstitutionstotal=csubstitutionstotal+1
!if (csubstitutionstotal.eq.50) then
!    continue
!endif
if (cbd.eq.0) then
    cbdsubseq=cbdsubseq+1
endif
write(*,*) 'Elapsed for iteration',csubstitutionstotal,' is',time_middle-time_begin, ' seconds'
write(3,*) 'Elapsed for iteration',csubstitutionstotal,' is',time_middle-time_begin, ' seconds'
write(4,*) 'Elapsed for iteration',csubstitutionstotal,' is',time_middle-time_begin, ' seconds'
if (csubstitutionstotal.eq.niterover.OR.cbdsubseq.eq.niternotimproved) then
    goto 1001 !achieved a 'global' optimum
else
              !make update of the current step and move to
    !natsol
    do iii=1,p
        natsol(iii)=natcur(iii)
    enddo
    !natsol(1)=ii
    !natsol(2)=jj
    !natsol(3)=ll
    !nat
    do iii=1,p
        nat(iii)=natsol(iii)
    enddo
    !MaxScore
    MaxScore=MaxScore+dzmax
    write(*,32) MaxScore,dzmax,MaxScoreEver
    write(3,32) MaxScore,dzmax,MaxScoreEver
    write(4,32) MaxScore,dzmax,MaxScoreEver
    write(5,32) MaxScore,dzmax,MaxScoreEver    
32  format(1x,' MaxScore = ',f9.6,' dzmax = ',f9.6,' Max Score Ever = ',f9.6)          
    write(*,*) 'Attributes are', (natsol(iii),iii=1,p)
    write(3,*) 'Attributes are', (natsol(iii),iii=1,p)
    write(4,*) 'Attributes are', (natsol(iii),iii=1,p)
    !Update tabu history matrix Mat with natsol sorted as snatsol
    call bsort(p,natsol,snatsol)
    do j=1,p
            Mat(j,ntenure+csubstitutionstotal)=snatsol(j)    
    enddo      
    goto 999  !the next step of neighborhood BMNH search  
endif    
!finally
1001 continue
!bf
do iii=1,p
    if (y(id(natever(iii))).ge.0) then
        bf(iii)=1
    else
        bf(iii)=0
    endif
enddo
!Af
do j=1,p
   do jjj=1,p 
      Af(jjj,j)=(1-2*bf(jjj))*Z(j+1,id(natever(jjj)))
      !Af(2,j)=(1-2*bf(2))*Z(j,id(natsol(2)))
      !Af(3,j)=(1-2*bf(3))*Z(j,id(natsol(3)))
   enddo
enddo
!bbf
do j=1,p
    bbf(j)=b0*(-1+2*bf(j))*Z(1,id(natever(j)))
enddo

call gelim(p,Af,bbf,x)

call EvaluateScore1(x,T,p,score1)

ArgMax=x
MaxScoreF=MaxScoreEver
MaxScoreFF=score1

write(*,*) 'Argmax is ',(Argmax(j),j=1,p)
write(3,*) 'Argmax is ',(Argmax(j),j=1,p)
write(4,*) 'Argmax is ',(Argmax(j),j=1,p)
write(5,*) 'Argmax is ',(Argmax(j),j=1,p)
write(*,*) 'MaxScore Ever is ',MaxScoreF
write(3,*) 'MaxScore Ever is ',MaxScoreF
write(4,*) 'MaxScore Ever is ',MaxScoreF
write(5,*) 'MaxScore Ever is ',MaxScoreF
write(*,*) 'MaxScore at Argmax is ',MaxScoreFF
write(3,*) 'MaxScore at Argmax is ',MaxScoreFF
write(4,*) 'MaxScore at Argmax is ',MaxScoreFF
write(5,*) 'MaxScore at Argmax is ',MaxScoreFF
write(*,*) 'coefficient of variable 0 b(0) is',b0
write(3,*) 'coefficient of variable 0 b(0) is',b0
write(4,*) 'coefficient of variable 0 b(0) is',b0
write(5,*) 'coefficient of variable 0 b(0) is',b0
write(*,*) 'attributes are',(natever(iii),iii=1,p) 
write(3,*) 'attributes are',(natever(iii),iii=1,p) 
write(4,*) 'attributes are',(natever(iii),iii=1,p) 
write(5,*) 'attributes are',(natever(iii),iii=1,p) 
call cpu_time(time_end)
write(*,*) 'ntenure = ',ntenure,'niterover = ',niterover,'niternotimproved = ',niternotimproved
write(3,*) 'ntenure = ',ntenure,'niterover = ',niterover,'niternotimproved = ',niternotimproved
write(4,*) 'ntenure = ',ntenure,'niterover = ',niterover,'niternotimproved = ',niternotimproved
write(5,*) 'ntenure = ',ntenure,'niterover = ',niterover,'niternotimproved = ',niternotimproved
write(*,*) 'Time of computation was ',time_end-time_begin, ' seconds'
write(3,*) 'Time of computation was ',time_end-time_begin, ' seconds'
write(4,*) 'Time of computation was ',time_end-time_begin, ' seconds'
write(5,*) 'Time of computation was ',time_end-time_begin, ' seconds'


end program  


