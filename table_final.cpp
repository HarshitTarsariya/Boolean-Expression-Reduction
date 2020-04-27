#include<bits/stdc++.h>

using namespace std;

int *minterms,*dontcare,****tab,**binary,grouping_possible=1,positionofX=0,*reducedPIchart_X,**reducedPIchart_Y,**reducedPIchart,*RF;
int  no_of_leftPI,variables,no_of_leftMT,no_of_minterms,**most_probably_EPI,PotEPINo=0,minm=0,no_of_poss_essential_prime_impli;


void tobinary()
{
	int temp;
	for(int i=0;i<no_of_minterms;i++)
	{
		temp=minterms[i];
		for(int j=variables-1;j>=0;j--)
		{
			binary[i][j]=temp%2;
       		temp=temp/2;
		}
	}
}
int one_count(int *binary,int no)
{
	int count=0;
	for(int i=0;i<=no-1;i++)
	{
			if(binary[i]==1)
				count++;
	}
	return count;
}
/*Generates max combinations for different number of 1's upto variableCvariable*/
int combi(int variables,int i,int j)
{
	int num=1,deno=1;
	for(int k=variables;k>=variables-j+1-i;k--)
	{
		num*=k;
	}
	for(int k=j;k>=1;k--)
	{
		deno*=k;
	}
	return(num/deno);
}
int powerof2(int n)
{
	return(floor(log(n)/log(2))==(log(n)/log(2)));
}
void looping(int m)
{
	int n=m;
	for(RF[n]=0;RF[n]<no_of_leftPI;RF[n]++)
	{
		if(reducedPIchart[no_of_leftMT-1-n][RF[n]])
		{
			if(n>0)
			{
				m=n;
				m--;
				looping(m);
			}
			else if(n==0)
			{
				for(int i=0;i<no_of_leftMT;i++)
				{
					most_probably_EPI[PotEPINo][i]=RF[no_of_leftMT-1-i];
				}
				PotEPINo++;
			}
		}
	}
}

int power(int time)
{
	int result=1;
	while(time!=0)
	{
		result=result*2;
		time--;
	}
	return(result);
}

int main()
{
	int no_of_dontcare=0,m,place_where_bit_changes,checker,     	no_of_prime_impli=0;
		
	int    **essential_prime_implicants, 					no_of_essential_prime_impli;
	cout<<"Enter the number of Variables"<<endl;
	cin>>variables;
	while(variables<=0)
	{
		cout<<"Invalid number of variables"<<endl;
		cin>>variables;
	}

	cout<<"Enter the number of Min terms"<<endl;
	cin>>no_of_minterms;
	while(no_of_minterms<=0||no_of_minterms>=power(variables))
	{
		cout<<"Invalid number of minterms"<<endl;
		cin>>no_of_minterms;
	}
		
	cout<<"Enter the  number of  don't cares(if present else 0)"<<endl;
	cin>>no_of_dontcare;
	while(no_of_dontcare<0 )
	{
		cout<<"Invalid number of dontcare , dont care cannot be less than 1 or cannot be greater than number of all minterms"<<endl;
		cin>>no_of_dontcare;
	}

	cout<<"Enter the min_terms"<<endl;
	minterms=(int *)malloc((no_of_minterms + 				no_of_dontcare)*sizeof(int));
	for(int i=0;i<no_of_minterms;i++)
	{
		cin>>minterms[i];
		if(	minterms[i] <0 || minterms[i]>=power(variables))
		{
			cout<<"Invalid minterm,so enter again"<<endl;
			i=-1;
		}
	}

	if(no_of_dontcare!=0)
	{
		cout<<"Enter the  dont care"<<endl;
		dontcare=(int *)malloc(no_of_dontcare*sizeof(int));
		for(int i=0;i<no_of_dontcare;i++)
		{
			cin>>dontcare[i];
			if(	dontcare[i] <0 || 					    				dontcare[i]>=power(variables))
			{
				cout<<"Invalid don't care,so enter 						again"<<endl;
				i=-1;
			}
			else
				minterms[no_of_minterms+i]=dontcare[i];
		}
	}
	no_of_minterms+=no_of_dontcare;
	sort(dontcare,dontcare+no_of_dontcare);
	sort(minterms,minterms+no_of_minterms);
		
		
	binary=(int**)malloc(no_of_minterms*sizeof(int*));
	for(int i=0;i<=no_of_minterms;i++)        
		binary[i]=(int*)malloc((variables+4)*sizeof(int));
	tobinary();  		//converts min terms to its binary
		
	for(int i=0;i<no_of_minterms;i++)
	{
		binary[i][variables]=one_count(binary[i],variables);
		binary[i][variables+1]=0;
           binary[i][variables+2]=minterms[i];
		binary[i][variables+3]=minterms[i];
	}
		
	/*the base table on which everything is done is created*/
		
	tab=(int****)malloc((variables+1)*sizeof(int***));
	for(int i=0;i<variables+1;i++)
		tab[i]=(int***)malloc((variables+1-i)*sizeof(int**));
	
	for(int i=0;i<variables+1;i++)
	{
		for(int j=0;j<variables+1-i;j++)
		{
			tab[i][j]=(int**) 									malloc((combi(variables,i,j))*sizeof(int*));
				
			for(int k=0;k<combi(variables,i,j);k++)
				tab[i][j][k]=NULL;
				
		}
	}
		
	/*first group is created according to no. 1's(Below i no. of 1's)*/
	for(int i=0;i<=variables;i++)
	{
		for(int j=0,k=0;j<no_of_minterms;j++)
		{
			if(binary[j][variables]==i)
				tab[0][i][k++]=binary[j];
		}
	}
	/*All Groupings*/
		
	for(int i=0;i<variables+1;i++)
	{
		if(grouping_possible)
		{
			grouping_possible=0;
			for(int j=0;j<variables-i;j++)
			{
				m=0;
				for(int k=0;k<combi(variables,i,j);k++)
				{
					if(tab[i][j][k]!=NULL)
					{
						for(int t=  				 						0;t<combi(variables,i,j+1);t++)
							{
								if(tab[i][j+1][t]!=NULL && tab[i][j+1][t][variables+2+i]>tab[i][j][k][variables+2+i] && powerof2(tab[i][j+1][t][variables+2+i]-tab[i][j][k][variables+2+i]) )
								{
									positionofX=0-i;
									for(int n=1;n<=i;n++)
									for(int p=1;p<=i;p++)				
									{
										if(tab[i][j+1][t][variables+1+n]==tab[i][j][k][variables+1+p])
											positionofX++;	
									}
									if(positionofX==0)
									{
										grouping_possible=1;
										tab[i][j][k][variables+1]=1;
										tab[i][j+1][t][variables+1]=1;
										tab[i+1][j][m]=(int *)malloc((variables+4+i+pow(2,i+1))*sizeof(int));
										for(int n=0;n<=variables+1+i;n++)               
											tab[i+1][j][m][n]=tab[i][j][k][n];
										              
										tab[i+1][j][m][variables+3+i]=tab[i][j][k][variables+2+i];
										
										for(int n=variables+4+i;n<variables+4+i+pow(2,i+1);n++)
											tab[i+1][j][m][n]=0;

										place_where_bit_changes=log((tab[i][j+1][t][variables+2+i]-tab[i][j][k][variables+2+i]))/log(2);
										 
										tab[i+1][j][m][variables-1-place_where_bit_changes]=2;
										tab[i+1][j][m][variables+1]=0;
										
										tab[i+1][j][m][variables+2+i]=place_where_bit_changes;
										
										for(int p=0;p<pow(2,i);p++)
											tab[i+1][j][m][variables+4+i+p]=tab[i][j][k][variables+3+i+p]; 
              
										for(int p=pow(2,i);p<pow(2,i+1);p++)
											tab[i+1][j][m][variables+4+i+p]=tab[i][j+1][t][variables+3+i+p-(int)pow(2,i)];

										m++;
									}
								}
							}
						}
					}
				}
			}
		}
		int *numberof_timeterm_occur=(int*)malloc(pow(2,variables)*sizeof(int));
		for(int i=0;i<pow(2,variables);i++)
			numberof_timeterm_occur[i]=0;
		/*getting unselected terms*/
		
		int **unchecked_values_index=(int **)malloc(no_of_minterms*sizeof(int*));
		for(int i=0;i<no_of_minterms;i++)
			unchecked_values_index[i]=(int *)malloc(3*sizeof(int)); //stores i,j,k index of unselected terms
		
		for(int i=0;i<variables+1;i++)
			for(int j=0;j<variables+1-i;j++)    
				for(int k=0;k<combi(variables,i,j);k++) 
				{
					if(tab[i][j][k]!=NULL && tab[i][j][k][variables+1]==0)
					{	
						checker=0-pow(2,i); /*check if this PI is a duplicate*/
						for(int t=k-1;t>=0;t--) 
							if(checker!=0) 
							{
								checker=0-pow(2,i); 
								for(int m=0;m<pow(2,i);m++) 
									for(int n=0;n<pow(2,i);n++)
										if(tab[i][j][t][variables+3+i+m]==tab[i][j][k][variables+3+i+n])
											checker++;
							}
							if(checker!=0)
							{
								unchecked_values_index[no_of_prime_impli][0]=i; 
								unchecked_values_index[no_of_prime_impli][1]=j; 
								unchecked_values_index[no_of_prime_impli][2]=k; 
								no_of_prime_impli++; 
								for(int l=0;l<pow(2,i);l++) 
									numberof_timeterm_occur[tab[i][j][k][variables+3+i+l]]++;
							}
					}
				}
		
		for(int i=0;i<no_of_dontcare;i++) 
			numberof_timeterm_occur[dontcare[i]]=0;   		//Removing Dont care Minterms		
		essential_prime_implicants=(int **)malloc(no_of_minterms*sizeof(int*));  //Minterms which are essential and can be grouped with only way
		
		for(int i=0;i<pow(2,variables);i++)
			if(numberof_timeterm_occur[i]==1) 
				for(int j=0;j<no_of_prime_impli;j++) 
					for(int k=0;k<pow(2,unchecked_values_index[j][0]);k++) 
					{
						if(tab[unchecked_values_index[j][0]][unchecked_values_index[j][1]][unchecked_values_index[j][2]][variables+3+unchecked_values_index[j][0]+k]==i)
						{
							essential_prime_implicants[no_of_essential_prime_impli]=unchecked_values_index[j]; 
							for(int l=0;l<pow(2,unchecked_values_index[j][0]);l++) 
								numberof_timeterm_occur[tab[unchecked_values_index[j][0]][unchecked_values_index[j][1]][unchecked_values_index[j][2]][variables+3+unchecked_values_index[j][0]+l]]=0;
							no_of_essential_prime_impli++; 
							k=pow(2,unchecked_values_index[j][0]);							
						}
					}
			/*Reduced Prime Implicants chart*/
		 no_of_leftMT=0;
		for(int i=0;i<pow(2,variables);i++)
		if(numberof_timeterm_occur[i]!=0)
			no_of_leftMT++;
		
		reducedPIchart_X=(int *)malloc(no_of_leftMT*sizeof(int));
		
		for(int i=0;i<no_of_leftMT;i++)
			reducedPIchart_X[i]=-1;
		
		reducedPIchart_Y=(int **)malloc(no_of_prime_impli*sizeof(int*));

		for(int i=0;i<no_of_prime_impli;i++)
			reducedPIchart_Y[i]=NULL;
		reducedPIchart=(int **)malloc(no_of_leftMT*sizeof(int*));

		/* First Row, consist of the remaining minterms decimal indices*/
		for(int i=0,j=0;j<pow(2,variables);j++)
			if(numberof_timeterm_occur[j]!=0)
			{
				reducedPIchart_X[i]=j;
				i++;
			}
		/*This is the First Column, consist of the remaining PIs*/
		 no_of_leftPI=0;
		for(int i=0;i<no_of_prime_impli;i++)
		for(int j=0;j<pow(2,unchecked_values_index[i][0]);j++)
		{
			if(numberof_timeterm_occur[tab[unchecked_values_index[i][0]][unchecked_values_index[i][1]][unchecked_values_index[i][2]][variables+3+unchecked_values_index[i][0]+j]]!=0)
			{
				j=pow(2,unchecked_values_index[i][0]);
				reducedPIchart_Y[no_of_leftPI]=unchecked_values_index[i];
				no_of_leftPI++;
			}
		}

	/*reducedPIchart[i][j] represent the information of reduced PrimeImplicants chart(1 means selected and 0 means not selected)*/
		if(no_of_leftPI!=0)
		{
			for(int i=0;i<no_of_leftMT;i++)
				reducedPIchart[i]=(int *)malloc(no_of_leftPI*sizeof(int));
			
			for(int i=0;i<no_of_leftMT;i++)
				for(int j=0;j<no_of_leftPI;j++)
					reducedPIchart[i][j]=0;

			for(int i=0;i<no_of_leftPI;i++)
				for(int j=0;j<pow(2,reducedPIchart_Y[i][0]);j++)
					for(int k=0;k<no_of_leftMT;k++)
						if(tab[reducedPIchart_Y[i][0]][reducedPIchart_Y[i][1]][reducedPIchart_Y[i][2]][variables+3+reducedPIchart_Y[i][0]+j]==reducedPIchart_X[k])
						{
							reducedPIchart[k][i]=1;
						}
			/*Select the EssentialPrimeImplicants from the reduced PrimeImplicants chart*/
			RF=(int *)malloc(no_of_leftMT*sizeof(int)); 
			for(int i=0;i<no_of_leftMT;i++)
			{
				RF[i]=-1;
			}
			no_of_poss_essential_prime_impli=1;
			for(int i=0;i<no_of_leftMT;i++)
				no_of_poss_essential_prime_impli=no_of_poss_essential_prime_impli*numberof_timeterm_occur[reducedPIchart_X[i]];
			
			most_probably_EPI=(int **)malloc(no_of_poss_essential_prime_impli*sizeof(int*));

			for(int i=0;i<no_of_poss_essential_prime_impli;i++)
				most_probably_EPI[i]=(int *)malloc(no_of_leftMT*sizeof(int));
		
			looping(no_of_leftMT-1);
			
			int *NoOfPIForEPI=(int *)malloc(no_of_poss_essential_prime_impli*sizeof(int)); //No Of PIForEPI[i] will count how many PIs are//in each combination that covers allminterms */
		
			for(int i=0;i<no_of_poss_essential_prime_impli;i++)
				NoOfPIForEPI[i]=0;

			for(int i=0;i<no_of_poss_essential_prime_impli;i++)
				for(int j=0;j<no_of_leftMT;j++)
					if(most_probably_EPI[i][j]!=-1)
					{
						NoOfPIForEPI[i]++;
						for(int k=j+1;k<no_of_leftMT;k++)
							if(most_probably_EPI[i][k]==most_probably_EPI[i][j])
								most_probably_EPI[i][k]=-1;
					}		
			/*combination which require the least number of PIs to cover all minterms*/
			for(int i=1;i<no_of_poss_essential_prime_impli;i++)
				if(NoOfPIForEPI[i]<NoOfPIForEPI[minm])
					minm=i;
			for(int i=0;i<no_of_leftMT;i++)
				if(most_probably_EPI[minm][i]!=-1)
					essential_prime_implicants[no_of_essential_prime_impli++]=reducedPIchart_Y[most_probably_EPI[minm][i]];
				
			/**FINAL RESULT*/
			
			printf("\nReduced Equation \n  \n");
			for(int i=0;i<no_of_essential_prime_impli;i++)
			{
				for(int j=0;j<variables;j++)
				{
					if(tab[essential_prime_implicants[i][0]][essential_prime_implicants[i][1]][essential_prime_implicants[i][2]][j]==1)
						printf("%c",65+j);
					else
						if(tab[essential_prime_implicants[i][0]][essential_prime_implicants[i][1]][essential_prime_implicants[i][2]][j]==0)
							printf("%c'",65+j);
				}
				if(i<no_of_essential_prime_impli-1)
					printf("+");
			}
			printf("\n\nPress key to exit!!!"); char t;
			t=getchar();
			t=getchar();
			return 0;
		}
		else
		{
			printf("\nReduced Equation \n  \n");
			for(int i=0;i<no_of_essential_prime_impli;i++)
			{
				for(int j=0;j<variables;j++)
				{
					if(tab[essential_prime_implicants[i][0]][essential_prime_implicants[i][1]][essential_prime_implicants[i][2]][j]==1)
						printf("%c",65+j);
					else
						if(tab[essential_prime_implicants[i][0]][essential_prime_implicants[i][1]][essential_prime_implicants[i][2]][j]==0)
							printf("%c'",65+j);
				}
				if(i<no_of_essential_prime_impli-1)
					printf("+");
			}
			printf("\n\nPress key to exit!!!");char t;
			t=getchar();
			t=getchar();
			return 0;
		}
	}









