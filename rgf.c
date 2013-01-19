#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define MAX_SIZE 5000
#define RED (10*KMAX)

struct dist_name{
  long long k;
  double p_raw;
  double c;
} dist_k[MAX_SIZE];

long long N=0,M=0;
double b,r,A,r_low,r_high,KMAX;
long long raw_dat[MAX_SIZE];

double relation_randb(double _b);
double det_kc(long long kc);
double det_A();

double relation_randb(double _b)
{
  long long k;
  double sum_1=0,sum_2=0;

  for(k=1;k<RED;k++)
    {
      sum_1+=exp(-_b*(double)k)/pow((double)k,(r-1));
      sum_2+=exp(-_b*(double)k)/pow((double)k,r);
    }

  return (sum_1/sum_2-(double)M/N);
}

double det_kc(long long kc)
{
  double temp=0;
  long long i;
  for(i=kc;i<RED;i++)
    temp+=A*exp(-b*(double)i)/pow((double)i,r);
  return (1./N-temp);
}

double det_A()
{
  double _sum=0;
  long long k;
  
  for(k=1;k<RED;k++)
    {
      _sum+=exp(-b*(double)k)/pow((double)k,r);
    }


  return 1./_sum;
}

int main (int argc, char *argv[])
{
  double mean_k;
  long long kc,k;

  if(argc!=6)
    {
      fprintf(stderr,"command line arguments; 1: M, 2:N, 3,4: initial range for searching r, 5:kmax\n");
      exit(1);
    }
  else
    {
      sscanf(argv[1],"%ld",&M);
      sscanf(argv[2],"%ld",&N);
      sscanf(argv[3],"%lf",&r_low);
      sscanf(argv[4],"%lf",&r_high);      
      sscanf(argv[5],"%lf",&KMAX);      

    }

  fprintf(stderr,"%ld\n",M);

  /*
  M=(int)pow(2,28);
  N=66063782;
  mean_k=(double)M/N;
  r=1.6;
  */

  mean_k=(double)M/N;
  

  for(r=r_low;r<=r_high;r+=0.1)
    {
      double temp_b=1e-8,temp_b2=1e2,sign_b1,sign_b2,dif,p_b=0,sign1,sign2,temp,temp2;
      long long itemp,itemp2;
      sign_b1=relation_randb(temp_b);
      sign_b2=relation_randb(temp_b2);

      fprintf(stderr,"b:%le %le\n",temp_b,(sign_b1*sign_b2));

      if((sign_b1*sign_b2)<0)
	{
	  while(fabs(temp_b-temp_b2)>1e-8)
	    {
	      p_b=(temp_b+temp_b2)/2;

	      sign_b1=relation_randb(temp_b);
	      sign_b2=relation_randb(p_b);

	      if((sign_b1*sign_b2)<0)
		{
		  temp_b2=p_b;
		}
	      else
		{
		  temp_b=p_b;
		}
	      fprintf(stderr,"b:%le %le\n",temp_b,(sign_b1*sign_b2));
	    }
	  b=p_b;
	}
      else goto det_b;

      fprintf(stderr,"b:%le\n",b);

    det_b:;

      if(p_b==0)
	{
	  fprintf(stderr,"Cannot find b value\n");
	  goto p_end;
	}

      A=det_A();
      long long index=0;

      itemp=1;
      sign1=det_kc(itemp);

      itemp2=M;
      sign2=det_kc(itemp2);

      if((sign1*sign2)<0)
	{
	  long long p_kc;

	  while(fabs(itemp-itemp2)>1)
	    {
	      p_kc=(long long)(itemp+itemp2)/2;

	      sign1=det_kc(itemp);
	      sign2=det_kc(p_kc);

	      if((sign1*sign2)<0)
		{
		  itemp2=p_kc;
		}
	      else
		{
		  itemp=p_kc;
		}
	      fprintf(stderr,"kc:%d\n",p_kc);
	    }
	  kc=p_kc;
	}
      else
	{
	  fprintf(stderr,"Cannot find kc value\n");
	  goto p_end;

	}

      temp=0;
      temp2=0;

      for(k=kc;k<RED;k++)
	{
	  temp+=A*exp(-b*(double)k)/pow((double)k,r);
	  temp2+=(double)k*A*exp(-b*(double)k)/pow((double)k,r);
	}
      fprintf(stderr,"%le %le %le %le \n",r,fabs(KMAX-(temp2/temp)),A,b);   
      fprintf(stdout,"%le %le %le %le \n",r,fabs(KMAX-(temp2/temp)),A,b);
    p_end:;
    }


  return 0;
}
