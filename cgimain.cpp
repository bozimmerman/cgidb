/*
   Copyright 2000-2005 Bo Zimmerman

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#define _CRT_SECURE_NO_WARNINGS 1

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include<unistd.h>
#include <fcntl.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>


char *myltoa(long x, char *mybuf, int len)
{
	char buf[50];
	char *nums="0123456789";
	int pos=0;
	buf[0]=0;
	if(x==0)
	{
		buf[pos]='0';
		pos++;
		buf[pos]=0;
	}
	else
	while(x>0)
	{
		int y=x%10;
		buf[pos]=nums[y];
		x=(x-y)/10;
		pos++;
		buf[pos]=0;
	}
	size_t i=0;
    strcpy(mybuf,buf);
	return mybuf;
}

char *malltoa(long x)
{
	char *buf;
	buf = (char *)malloc(50);
	buf=myltoa(x,buf,10);
	return buf;
}

class string
{
  private:
    char *s;
  public:
    string(char *copy);			// copy constructor
    string(int size);                   // sized string constructor
    string(string& copy);               // copy constructor for other strs
    void init(int size);		// alternate constructor
    void init(char *copy);		// alternate copy constructor
    string();                           // void empty string constructor
    ~string();                          // destructor
    char& operator [](int i);           // implements character references
    string& operator =(char *copy);     // assignment to character arrays
    string& operator =(string& copy);   // assignment to other strings
    char *operator +(char *s2);         // add a string to a char array
    char *operator +(char c);           // add a char to a char array
    char *operator +(string& s2);       // add a string to a string

    int len();                          // get the length
	char *buf();
    char *operator *();                 // if you need func.s from string.h
    char *operator %(string& s2);       // return string position
    char *operator %(char *s2);         // return char array position
    int operator ==(char *s2);          // string equal to char array
    int operator ==(string& s2);        // string equal to string
    int operator !=(char *s2);          // string not equal to char array
    int operator !=(string& s2);        // string not equal to string
    int operator >(char *s2);           // string greater than char array
    int operator >(string& s2);         // string greater than string
    int operator <(char *s2);           // string less than char array
    int operator <(string& s2);         // string less than string
    int operator >=(char *s2);          // string greater/equal char array
    int operator >=(string& s2);        // string greater/equal string
    int operator <=(char *s2);          // string less/equal to char array
    int operator <=(string& s2);        // string less/equal to string
    char *operator -(int x);		// remove x chars from front
    char *operator -(string& s2);       // remove s2 from string
    char *operator -(char *s2);		// remove char s2 from string
	int Find(char c);				// return the index of char c in string
	int Find(char *s2);				// return the index of char c in string
	char *Right(int numchars);				// return the index of char c in string
	char *Left(int numchars);				// return the index of char c in string
	char *Mid(int numchars);				// return the index of char c in string
    friend std::ostream &operator <<(std::ostream& o, string& s);   // stream out
    friend std::istream &operator >>(std::istream& o, string& s);   // stream in
};

static int fout=-1;
FILE *out;

void output(char *c)
{
	printf("%s",c);
	return;
}

class DATA
{
	public:
		DATA(char *DatabaseName, char *TagsPath, char *DataPath);
		~DATA();
		char *DataName;
		char *TagsPath;
		char *DataPath;
		DATA& operator=(DATA& data);

		char *AddTag(char *newTAG, long newLen);
		long DelTag(char *oldTAG);
		long GetTagMax(char *findTAG);
		long WhichCol(char *findTAG);
		long ModifyTag(char *findTAG, char *newTAG, long newLen);
		char *GetTag(long Col);
		long GetNumCols();
		long LoadTags();
		long SaveTags();

		char *AddCol(long Key, char *findTAG, char *newData);
		char *ClrCol(long Key, char *findTAG);
		char *GetCol(long Key, char *findTAG);
		long AddRow();
		long AddRow(char **ROW);
		long AddRow(char **ROW, long Key);
		long DelRow(long Key);
		char **GetRow(long Key);
		long KeyFromRow(long Row);
		long RowFromKey(long Key);
		long GetNumRows();
		long LoadData();
		long SaveData();
	private:
		char **cpTAGS;
		long *lpMax;
		long nTAGS;
		char ***cpDATA;
		long *lpKEYS;
		long nDATA;
		void ClrTags();
		long FreeData(long rows, long cols, char ***freeme, long *freemetoo);
};

struct DBTYPE {
	char PWORD[20];
	long NumTables;
	char TableNames[20][30];
	char StructPaths[20][80];
	char DataPaths[20][80];
	DATA *Tables[20];
};

int readLine(FILE *f, char *here)
{
	char c=fgetc(f);
	int pos=0;
	while((c>-1)&&(c!=10))
	{
		if((c!=10)&&(c!=13))
			here[pos++]=c;
		c=fgetc(f);
	}
	here[pos]=0;
	return c;
}

// Globals
string OLDQSTRING("");
char **SELECTEDROW = 0;
long PageMode = 0;


void UpdateData(DATA *data);
void SortBy(DATA *data, char *QUERYSTRING);
void Select(DATA *data, char *QUERYSTRING);
void Field(DATA *data, char *QUERYSTRING);
void RawSpitData(DATA *data);
void List(DATA *data, char *QUERYSTRING);
void Details(DATA *data, char *QUERYSTRING);
void EditAll(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void EditDetails(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void AddDetails(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void ChangeRow(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void AddRow(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void DelRow(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);

// TagReqs functions
void RawSpitTags(DATA *data);
void TagDetails(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void DelTag(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void EditTags(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void UpdateTags(DATA *data);

//DBreqs functions
long ReadDB(DATA **data, struct DBTYPE *DBmain, char *DBNAME);
void DoPage(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void Setup(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void SetupUpdate(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
void InitializeSystem();
char *Encrypt(char *ENCRYPTME);
char *Decrypt(char *DECRYPTME);
long ReadINI(DBTYPE *DBmain, char *pospass);
void ParseRequests(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING);
long BuildTableCols(char ***CTags, char ***CPcts, DATA *data, char *QUERYSTRING);
void FreeTableCols(char ***ColTags, char ***ColPcts, DATA *data);

DATA::DATA(char *DatabaseName, char *Tags_Path, char *Data_Path)
{
	cpTAGS = 0;
	lpMax = 0;
	nTAGS = 0;
	cpDATA = 0;
	nDATA = 0;
	lpKEYS = 0;
	if((DatabaseName!=0)&&(Data_Path!=0)&&(Tags_Path!=0))
	{
		DataName = (char *)malloc((strlen(DatabaseName)+1)*sizeof(char));
		TagsPath = (char *)malloc((strlen(Tags_Path)+1)*sizeof(char));
		DataPath = (char *)malloc((strlen(Data_Path)+1)*sizeof(char));

		strcpy(DataName,DatabaseName);
		strcpy(TagsPath,Tags_Path);
		strcpy(DataPath,Data_Path);
	}
}


void DATA::ClrTags()
{
	if(nTAGS>0)
	{
		for(long i=0;i<nTAGS;i++)
			free(cpTAGS[i]);
		free(cpTAGS);
		free(lpMax);
		cpTAGS = 0;
	}
};

long DATA::FreeData(long rows, long cols, char ***freeme, long *freemetoo)
{
	if(rows>0)
	{
		for(long x=0;x<rows;x++)
		{
			for(long i=0;i<cols;i++)
				free(freeme[x][i]);
			free(freeme[x]);
		}
		free(freeme);
		free(freemetoo);
	}
	return 0;
}


DATA::~DATA()
{
	cpDATA = (char ***)FreeData(nDATA, nTAGS, cpDATA, lpKEYS);
	DATA::ClrTags();
	nTAGS=0;
	nDATA=0;
	if(DataName!=0)
		free(DataName);
	DataName=0;
	if(TagsPath!=0)
		free(TagsPath);
	TagsPath=0;
	if(DataPath!=0)
		free(DataPath);
	DataPath=0;
};

DATA& DATA::operator=(DATA& newdata)
{
	if((newdata.DataName!=0)&&(newdata.TagsPath!=0)&&(newdata.DataPath!=0))
	{
		if (this->DataName!=0) free(this->DataName);
		this->DataName = (char *)malloc((strlen(newdata.DataName)+1)*sizeof(char));
		if (this->TagsPath!=0) free(this->TagsPath);
		this->TagsPath = (char *)malloc((strlen(newdata.TagsPath)+1)*sizeof(char));
		if (this->DataPath!=0) free(this->DataPath);
		this->DataPath = (char *)malloc((strlen(newdata.DataPath)+1)*sizeof(char));

		strcpy(this->DataName,newdata.DataName);
		strcpy(this->TagsPath,newdata.TagsPath);
		strcpy(this->DataPath,newdata.DataPath);
	};

	if (this->nDATA > 0)
		this->cpDATA = (char ***)this->FreeData(this->nDATA, this->nTAGS, this->cpDATA, this->lpKEYS);
	this->nDATA=0;
	if (this->nTAGS > 0)
		this->ClrTags();
	this->nTAGS=0;

	for(long t=0;t<newdata.GetNumCols();t++)
	{
		if (newdata.GetTag(t)>0)
			this->AddTag(newdata.GetTag(t),newdata.GetTagMax(newdata.GetTag(t)));
	}
	for(long r=0;r<newdata.GetNumRows();r++)
		if (newdata.GetRow(newdata.KeyFromRow(r))>0)
			this->AddRow(newdata.GetRow(newdata.KeyFromRow(r)),newdata.KeyFromRow(r));

	return *this;
}


char *DATA::AddTag(char *newTAG, long newLen)
{
	char **NEWcpTAGS;
	long *FakeKeys;
	long *NEWclMax;
	char ***NEWcpDATA;

	if(newTAG != 0)
		if(strlen(newTAG)>0)
		{
			// is it already there?
			if(WhichCol(newTAG)>=0)
				return 0;
			NEWcpTAGS = (char **)malloc ((nTAGS+1) * sizeof(char *));
			NEWclMax = (long *)malloc((nTAGS+1) * sizeof(long));
			for(long i=0;i<nTAGS;i++)
			{
				NEWcpTAGS[i] = (char *)malloc ((strlen(cpTAGS[i])+1) * sizeof(char));
				strcpy(NEWcpTAGS[i],cpTAGS[i]);
				NEWclMax[i] = lpMax[i];
			}
			NEWcpTAGS[nTAGS] = (char *)malloc ((strlen(newTAG)+1) * sizeof(char));
			strcpy(NEWcpTAGS[nTAGS],newTAG);
			NEWclMax[nTAGS] = newLen;
			ClrTags();
			cpTAGS = NEWcpTAGS;
			lpMax = NEWclMax;
			// new tags list, max list built

			// now rebuild the data. argh.
			NEWcpTAGS = 0;
			if (nDATA>0)
			{
				NEWcpDATA= (char ***)malloc(nDATA*sizeof(char ***));
   				for(long R=0;R<nDATA;R++)
				{
					NEWcpDATA[R] = (char **)malloc((nTAGS+1)*sizeof(char **));
					for (long C=0;C<nTAGS;C++)
					{
						NEWcpDATA[R][C] = (char *)malloc((strlen(cpDATA[R][C])+1)*sizeof(char));
						strcpy(NEWcpDATA[R][C],cpDATA[R][C]);
					}
					NEWcpDATA[R][nTAGS] = (char *)malloc(5 * sizeof(char));
					strcpy(NEWcpDATA[R][nTAGS],"");
				}
				FakeKeys = (long *)malloc(100);
				cpDATA = (char ***)FreeData(nDATA, nTAGS, cpDATA, FakeKeys);
				cpDATA = NEWcpDATA;
				NEWcpDATA = 0;
			}
			nTAGS++;
		}

	return newTAG;
};

long DATA::DelTag(char *oldTAG)
{
	char **NEWcpTAGS;
	long *NEWlpMax;
	char ***NEWcpDATA;
	long *FakeKeys;
	int FLAG = 0;
	int genloop;
	long oldCol = -1;

	if(oldTAG != 0)
		if(strlen(oldTAG)>0)
		{
			if ((oldCol=WhichCol(oldTAG))>=0)
			{
				// first rebuild the tags list
				NEWcpTAGS = (char **)malloc ((nTAGS-1) * sizeof(char *));
				NEWlpMax = (long *)malloc ((nTAGS-1) * sizeof(long));
				genloop = 0;
				for(long i=0;i<nTAGS;i++)
					if(strcmp(cpTAGS[i],oldTAG)!=0)
					{
						NEWcpTAGS[genloop] = (char *)malloc ((strlen(cpTAGS[i])+1) * sizeof(char));
						strcpy(NEWcpTAGS[genloop],cpTAGS[i]);
						NEWlpMax[genloop] = lpMax[i];
						genloop++;
					}
				ClrTags();
				cpTAGS = NEWcpTAGS;
				lpMax = NEWlpMax;
				NEWcpTAGS = 0;
				// ok, tags list rebuilt

				// now rebuild the data, if necessary
				if (nDATA>0)
				{
					NEWcpDATA= (char ***)malloc(nDATA*sizeof(char ***));
   					for(long R=0;R<nDATA;R++)
					{
						NEWcpDATA[R] = (char **)malloc((nTAGS-1)*sizeof(char **));
						genloop = 0;
						for (long C=0;C<nTAGS;C++)
							if(C!=oldCol)
							{
								NEWcpDATA[R][genloop] = (char *)malloc((strlen(cpDATA[R][C])+1)*sizeof(char));
								strcpy(NEWcpDATA[R][genloop],cpDATA[R][C]);
								genloop++;
							};
					};
					FakeKeys = (long *)malloc(100);
					cpDATA = (char ***)FreeData(nDATA, nTAGS, cpDATA, FakeKeys);
					cpDATA = NEWcpDATA;
					NEWcpDATA = 0;
				}
				nTAGS--;
				return 1;
			}
		}
	return 0;
};

long DATA::WhichCol(char *findTAG)
{
	long INDEX = -1;

	if(findTAG != 0)
		if(strlen(findTAG)>0)
		{
			for(long i=0;i<nTAGS;i++)
				if(strcmp(cpTAGS[i],findTAG)==0) INDEX=i;
		};
	return INDEX;
}

long DATA::GetTagMax(char *findTAG)
{
	long Max = WhichCol(findTAG);


	if (Max>=0)
		return lpMax[Max];
	else
		return -1;
}

char *DATA::GetTag(long Col)
{
	if((Col<nTAGS)&&(Col>=0))
		return cpTAGS[Col];
	else
		return 0;
}

long DATA::ModifyTag(char *findTAG, char *newTAG, long newLen)
{
	char NEWTAG[256];
	long INDEX = WhichCol(findTAG);

	if ((newTAG != 0)&&(INDEX>=0)&&(INDEX<nTAGS))
	{
		strcpy(NEWTAG,newTAG);
		free(cpTAGS[INDEX]);
		cpTAGS[INDEX] = (char *)malloc((strlen(NEWTAG)+1) * sizeof(char));
		strcpy(cpTAGS[INDEX],NEWTAG);
		lpMax[INDEX] = newLen;
		return -1;
	}
	return 0;
}


long DATA::LoadTags()
{
	char  NextTag[256];
	char  NextMax[256];
	long l=0;
	FILE *f;

	if(TagsPath==0)
		return -1;

	f=fopen(TagsPath,"r");
	if(f>=0)
	{
		while (!feof(f))
		{
			if(readLine(f,NextTag)<0) break;

			for(l=0;NextTag[l]!=0;l++);
			if(NextTag[l-1]==10)
				NextTag[l-1]=0;
			if(readLine(f,NextMax)<0) break;
			for(l=0;NextMax[l]!=0;l++);
			if(NextMax[l-1]==10)
				NextMax[l-1]=0;
			if (strcmp(AddTag(NextTag,atol(NextMax)),NextTag)!=0)
					return -3;
		}
		fclose(f);
	}
	else
		return -2;
	return 1;
};

long DATA::SaveTags()
{
	char *CR = "\n";
	char NewMax[256];
	FILE *f;

	if(TagsPath==0)
		return -1;
	if (nTAGS<1) return -1;

	f=fopen(TagsPath,"w");
	if(f>=0)
	{
		for(long R=0;R<nTAGS;R++)
		{
			fprintf(f,"%s\n",cpTAGS[R]);
			fprintf(f,"%s\n",myltoa(lpMax[R],NewMax,10));
		}
		fclose(f);
	}
	else
		return -2;
	return 1;
};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
// Here begins the Data related functions
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
long DATA::RowFromKey(long Key)
{
	if (nDATA>0)
		for(long i=0; i<nDATA; i++)
			if(Key==lpKEYS[i]) return i;
	return -1;
}


long DATA::KeyFromRow(long Row)
{
	if ((Row>=0)&&(Row<nDATA))
		 return lpKEYS[Row];
	return -1;
}


char *DATA::AddCol(long Key, char *findTAG, char *newData)
{
	long Col;
	long Row = RowFromKey(Key);

	if((Row<nDATA)&&(newData!=0)&&(nTAGS>0)&&(Row>=0))
		if((Col=WhichCol(findTAG))>=0)
		{
			if(cpDATA[Row][Col]!=0)	free(cpDATA[Row][Col]);
			cpDATA[Row][Col] = (char *)malloc((strlen(newData)+1)*sizeof(char));
			strcpy(cpDATA[Row][Col],newData);
			return newData;
		}
	return 0;
};

char *DATA::ClrCol(long Key, char *findTAG)
{
	long Col;
	char *oldData = 0;
	long Row = RowFromKey(Key);

	if((Row<nDATA)&&(nTAGS>0)&&(Row>=0))
		if((Col=WhichCol(findTAG))>=0)
		{
			if(cpDATA[Row][Col]!=0)
			{
				oldData = (char *)malloc(strlen(cpDATA[Row][Col]+1)*sizeof(char));
				strcpy(oldData,cpDATA[Row][Col]);
				free(cpDATA[Row][Col]);
				cpDATA[Row][Col] = (char *)malloc(sizeof(char));
			}
			cpDATA[Row][Col][0]=0;
			return oldData;
		}
	return 0;
};

char *DATA::GetCol(long Key, char *findTAG)
{
	long Col;
	long Row = RowFromKey(Key);

	if((Row<nDATA)&&(nTAGS>0)&&(Row>=0)&&(Row>=0))
		if((Col=WhichCol(findTAG))>=0)
		{
			if(cpDATA[Row][Col]!=0)
				return cpDATA[Row][Col];
			else
				return "";
		};
	return 0;
};



long DATA::AddRow()
{
	char ***NEW=0;
	long *NEWKEYS=0;
	long ChkR = 0;
	long R;

	NEW = (char ***)malloc((nDATA+1)*sizeof(char **));
	NEWKEYS = (long *)malloc((nDATA+1)*sizeof(long));

	if (nDATA>0)
	{
   		for(R=0;R<nDATA;R++)
		{
			NEW[ChkR] = cpDATA[R];
			NEWKEYS[ChkR] = lpKEYS[R];
 			ChkR++;
		}
		free(cpDATA);
		free(lpKEYS);
	}
	NEW[ChkR] = (char **)malloc(nTAGS*sizeof(char **));
	for (long C=0;C<nTAGS;C++)
	{
		NEW[ChkR][C] = (char *)malloc(sizeof(char));
		NEW[ChkR][C][0]=0;
	}
	NEWKEYS[ChkR] = ChkR;

	cpDATA = NEW;
	lpKEYS = NEWKEYS;

	nDATA++;
	return 1;
};

long DATA::AddRow(char **ROW)
{
	char ***NEW=0;
	long *NEWKEYS=0;
	long ChkR = 0;
	long R;


	if (ROW != 0)
	{
		NEW = (char ***)malloc((nDATA+1)*sizeof(char **));
		NEWKEYS = (long *)malloc((nDATA+1)*sizeof(long));

		if (nDATA>0)
		{
   			for(R=0;R<nDATA;R++)
			{
				NEW[ChkR] = cpDATA[R];
				NEWKEYS[ChkR] = lpKEYS[R];
 				ChkR++;
			}
			free(cpDATA);
			free(lpKEYS);
		}
		NEW[ChkR] = (char **)malloc(nTAGS*sizeof(char **));
		for (long C=0;C<nTAGS;C++)
		{
			NEW[ChkR][C] = (char *)malloc((strlen(ROW[C])+1)*sizeof(char));
			strcpy(NEW[ChkR][C],ROW[C]);
		}
		NEWKEYS[ChkR] = ChkR;

		cpDATA = NEW;
		lpKEYS = NEWKEYS;
		nDATA++;

		return 1;
	}
	return 0;
};

long DATA::AddRow(char **ROW, long Key)
{
	long val = this->AddRow(ROW);

	if (val==1)
		lpKEYS[nDATA-1]=Key;
	return val;
};

long DATA::DelRow(long Key)
{
	long Row = RowFromKey(Key);
	char ***NEW=0;
	long *NEWKEYS=0;
	long ChkR = 0;
	long R;

	if((Row>=nDATA)||(Row<0)||(nDATA==0))
		return 0;

	NEW = (char ***)malloc((nDATA-1)*sizeof(char **));
	NEWKEYS = (long *)malloc((nDATA-1)*sizeof(long));

   	for(R=0;R<nDATA;R++)
		if(Row!=R)
		{
			NEW[ChkR] = cpDATA[R];
			NEWKEYS[ChkR] = lpKEYS[R];
 			ChkR++;
		};
	free(cpDATA);
	free(lpKEYS);

	cpDATA = NEW;
	lpKEYS = NEWKEYS;
	nDATA--;
	return 1;
};

char **DATA::GetRow(long Key)
{
	long Row = RowFromKey(Key);

	if((Row<nDATA)&&(Row>=0))
		return cpDATA[Row];
	return 0;
};

long DATA::LoadData()
{
	char  NextCol[1024];
	long C = 0;
	long l=0;
	FILE *f;

	if(DataPath==0)
		return -1;
	if (nTAGS<1) return -1;

	f=fopen(DataPath,"r");
	if(f>=0)
	{
		while (!feof(f))
		{
			if(readLine(f,NextCol)<0) break;
			AddRow();
			for(l=0;NextCol[l]!=0;l++);
			if(NextCol[l-1]==10)
				NextCol[l-1]=0;
			if(strcmp(AddCol(nDATA-1, cpTAGS[0], NextCol),NextCol)!=0)
				return 0;
			for(C=1;C<nTAGS;C++)
			{
				if(!feof(f))
				{
					if(readLine(f,NextCol)<0) break;
					for(l=0;NextCol[l]!=0;l++);
					if(NextCol[l-1]==10)
						NextCol[l-1]=0;
					if(strcmp(AddCol(nDATA-1, cpTAGS[C], NextCol),NextCol)!=0)
						return -3;
				};
			}
		}
		fclose(f);
	}
	else
		return -2;
	return 1;
};

long DATA::SaveData()
{
	char *CR = "\n";
	FILE *f;

	if(DataPath==0)
		return -1;
	if (nTAGS<1) return -1;

	f=fopen(DataPath,"w");
	if(f>=0)
	{
		for(long R=0;R<nDATA;R++)
			for (long C=0;C<nTAGS;C++)
			{
				fprintf(f,"%s\n",cpDATA[R][C]);
			};
		fclose(f);
	}
	else
		return -2;
	return 1;
}

long DATA::GetNumCols()
{
	return nTAGS;
}

long DATA::GetNumRows()
{
	return nDATA;
}

static char BADCHR[24][4]={"+", "%09", "%0A", "%2F", "%7E", "%3A", "%3B", "%40", "%26", "%28", "%29", "%22", "%3C", "%3D", "%3E", "%24", "%2C", "%3F", "%21", "%23", "%2B", "%25", "%20"};
static char GODCHR[24] = {' ', '\t',  '\n',  '/',   '~',   ':',   ';',   '@',   '&',   '(',   ')',   '\'',   '<',   '=',   '>',   '$',   ',',   '?',   '!',   '#',   '+',   '%',  ' '};

char *ParseQuery(char *QSTRING, char *TAG)
{
	string NewQ(QSTRING);
	string NewTag(TAG);
	string FindTag("&");
	char *NewerQ;
	long locs;


	if (NewTag.Right(1)!="=") NewTag = NewTag + "=";
	FindTag = FindTag + NewTag;
	locs = NewQ.Find(FindTag.buf());
	if (locs<0)
	{
		FindTag=NewTag;
		locs = NewQ.Find(FindTag.buf());
		if(locs!=0) locs=-1;
	}

	if (locs >=0)
	{
		locs = locs + FindTag.len();
		NewQ = NewQ.Mid(locs);
		if ((locs = NewQ.Find('&'))>=0)
			NewQ = NewQ.Left(locs);
		string NewerQ = NewQ;
		long locs;

		for(long i=0;i<23;i++)
			while((locs = NewerQ.Find(BADCHR[i]))>=0)
			{
				char *left=NewerQ.Left(locs);
				char *right=NewerQ.Mid(locs+(int)strlen(BADCHR[i]));
				NewerQ = left;
				NewerQ = NewerQ + GODCHR[i];
				NewerQ = NewerQ + right;
			}
		NewQ=NewerQ;
	}
	else
		NewQ = "";

	NewerQ = (char *)malloc(sizeof(char)*(1+NewQ.len()));
	strcpy(NewerQ,NewQ.buf());
	return NewerQ;
}

long TrueCondition(DATA *data, long r, char *QUERYSTRING)
{
	long Trueness=0;
	char *ThisTag;
	char *ThisVal = 0;
	char *NxtVal;
	char KEYVAL[10];
	char *MyKey=ParseQuery(QUERYSTRING,"DBKEY");
	char *MyDB=ParseQuery(QUERYSTRING,"DBNAME");

	Trueness=0;
	if (MyKey[0]!=0)
	{
		myltoa(data->KeyFromRow(r),KEYVAL,10);
		if (strcmp(KEYVAL,MyKey)==0)
			Trueness=1;
	}
	if(MyDB[0]!=0)
	{
		if(strcmp(MyDB,data->DataName)==0)
			Trueness=1;
	}

	for(long C=0;C<data->GetNumCols();C++)
	{
		ThisVal=ParseQuery(QUERYSTRING,data->GetTag(C));
		ThisTag=ThisVal;
		while((ThisTag!=0)&&(ThisTag[0]!=0))
		{
			if((NxtVal=strstr(ThisVal,".OR."))>0)
			{
				NxtVal[0]=0;
				NxtVal+=4;
			}
			if((ThisTag[0]!=0)&&(data->KeyFromRow(r)>=0))
			{
				if(strcmp(ThisTag,data->GetCol(data->KeyFromRow(r),data->GetTag(C)))==0)
					Trueness=1;
			}
			ThisTag = NxtVal;
		}
		free(ThisVal);
		ThisVal=0;
	}
	free(MyKey);
	return Trueness;
}

char *DPGatherUntilYouGetOneOf(FILE *f, string These)
{
	string Buildable("");
	char ThisChr[2];
	char *MyLegit;
	long fer;

	while(((fer=fscanf(f,"%c",&ThisChr[0]))>=0)&&(These.Find(ThisChr[0])<0))
		Buildable = Buildable + ThisChr[0];

	if (fer<0)
	{
		return 0;
	}

//	FILE->Seek(pos,CFile::begin);
	MyLegit = (char *)malloc(sizeof(char) * (1+ Buildable.len()));
	strcpy(MyLegit,Buildable.buf());

	return MyLegit;
}

long DPCOUTUntilYouGet(FILE *f, string This, long Skipping)
{
	string Buildable("");
	char ThisChr[2];
	long fer;
	long numtofire = 0;

	while((This != Buildable.Right(This.len()))&&((fer=fscanf(f,"%c",&ThisChr[0]))>=0))
		Buildable =Buildable+ThisChr[0];

	if (Skipping==0)
	{
		numtofire = Buildable.len();
		if (fer>0) numtofire -= This.len();
		output(Buildable.Left(numtofire));
	}
	return fer;
}

long DPThrowAwayAllOf(FILE *f, string These)
{
	long pos;
	char ThisChr[2];
	long fer;

	pos = ftell(f);
	while(((fer=fscanf(f,"%c",&ThisChr[0]))>=0)&&(These.Find(ThisChr[0])>=0))
		pos = ftell(f);


	if (fer<0)
		return fer;

	fseek(f,pos,SEEK_SET);
	return fer;
}

void Select(DATA *data, char *QUERYSTRING)
{
	char *ThisTag;
	long FlagToStart = 0;
	long rows = 0;
	long C=0;
	char *MyKey=ParseQuery(QUERYSTRING,"DBKEY");
	DATA newdata(data->DataName,data->TagsPath,data->DataPath);
	for(C=0;C<data->GetNumCols(); C++)
		newdata.AddTag(data->GetTag(C),data->GetTagMax(data->GetTag(C)));

	rows = data->GetNumRows();
	if (rows == 0)
	{
		free(MyKey);
		return;
	}

	if(MyKey[0]!=0)	FlagToStart++;

	for(C=0;C<data->GetNumCols();C++)
	{
		ThisTag=ParseQuery(QUERYSTRING,data->GetTag(C));
		 if(ThisTag[0]!=0) FlagToStart++;
		free(ThisTag);
	}

	if (FlagToStart>0)
	{
		for(long r=0;r<rows;r++)
			if(TrueCondition(data,r,QUERYSTRING)!=0)
				newdata.AddRow(data->GetRow(data->KeyFromRow(r)),data->KeyFromRow(r));

		*data = newdata;
		if (data->GetNumRows()>0)
			SELECTEDROW = data->GetRow(data->KeyFromRow(0));
	}
	free(MyKey);
}

void SortBy(DATA *data, char *QUERYSTRING)
{
	DATA *sorted;
	char *Tag;
	long s=1;
	char *DFlag;
	long DFLAG=0;
	char *SearchTag;
	long FoundKey;
	long FoundRow;
	long RowKey;

	if (data->GetNumRows()==0) return;



	DFlag = ParseQuery(QUERYSTRING,"SORTORDER");
	if (strcmp(DFlag,"DESCENDING")==0)
		DFLAG=1;
	free(DFlag);

	Tag=ParseQuery(QUERYSTRING,"SORTBY");
	while (data->WhichCol(Tag)>=0)
	{
		sorted = new DATA(data->DataName,data->TagsPath,data->DataPath);
		for(long C=0;C<data->GetNumCols(); C++)
			sorted->AddTag(data->GetTag(C),data->GetTagMax(data->GetTag(C)));
		while(data->GetNumRows()>0)
		{
			SearchTag = (char *)malloc(sizeof(char) * (100+data->GetTagMax(Tag)));
			RowKey = data->KeyFromRow(0);
			FoundKey=RowKey;
			FoundRow=0;
			strcpy(SearchTag,data->GetCol(RowKey,Tag));
			for(long r=1; r<data->GetNumRows(); r++)
			{
				RowKey = data->KeyFromRow(r);
				if(((DFLAG==0)&&(strcmp(SearchTag,data->GetCol(RowKey,Tag))>0))
				|| ((DFLAG==1)&&(strcmp(SearchTag,data->GetCol(RowKey,Tag))<0)))
				{
					strcpy(SearchTag,data->GetCol(RowKey,Tag));
					FoundKey=RowKey;
					FoundRow=r;
				}
			}
			sorted->AddRow(data->GetRow(FoundKey),FoundKey);
			data->DelRow(FoundKey);
			free(SearchTag);
		}
		*data = *sorted;
		delete sorted;
		free(Tag);
		Tag=ParseQuery(QUERYSTRING,"ANDTHENBY");
	}
	free(Tag);
}

void DoPage(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *FileName = ParseQuery(QUERYSTRING,"INPUTFILE");
	FILE *f;
	long State=0;
	long IfState=0;
	int xx;
	string constOpen("<#");
	string constClose(">");

	long SeekBack=-1;
	long RowLoop = 0;
	long Skipping = 0;

	char *ThisCmd;
	char *ThisPCmd;

	Select(data,QUERYSTRING);
	SortBy(data,QUERYSTRING);

	DATA *backup = new DATA("","","");
	*backup = *data;

	if(FileName[0]==0)
	{
		output("<BR>Input file not found.<BR>\n");
		free(FileName);
		return;
	}

	if(fout>=0)	fprintf(out,"Start.\n");
	f=fopen(FileName,"r");
	if(f<=0)
	{
		output("<BR>Could not open input file '");
		output(FileName);
		output("'.<BR>\n");
		free(FileName);
		return;
	}

	PageMode=1;
	while(State>=0)
	switch(State)
	{
		case 0: // loop until '<#';
			xx=DPCOUTUntilYouGet(f, constOpen, Skipping);
			if (!xx)
			{
				State=-1;
			}
			else
				State=1;
			break;
		case 1: // expecting meta-command
			ThisPCmd = DPGatherUntilYouGetOneOf(f,constClose);
			if (ThisPCmd==0)
			{
				State=-1;
			}
			else
			{
				ThisCmd = ParseQuery(ThisPCmd,"REQ");

				if(strcmp(ThisCmd,"PARM")==0)
				{
					char *Parm = ParseQuery(ThisPCmd,"NAMED");
					if (Parm[0] != 0)
					{
						char *fieldData = ParseQuery(QUERYSTRING,Parm);
						if(fieldData[0] != 0)
							output(fieldData);
						free(fieldData);
						free(Parm);
					}
					else
					{
						output("** Unknown PARM NAMED: ");
						output(Parm);
						output("\n\n");
						free(Parm);
					}
				}
				else
				if(strcmp(ThisCmd,"ENDIF")==0)
				{
					Skipping = 0;
					IfState=0;
				}
				else
				if((strcmp(ThisCmd,"ELSE")==0)&&(IfState==1))
				{
					if (Skipping==0)
						Skipping = 1;
					else
						Skipping=0;
				}
				else
				if (Skipping == 0)
				{
					if(strcmp(ThisCmd,"ROWLOOP")==0)
					{
						SeekBack = ftell(f);
						RowLoop=0;
						if (data->GetNumRows()>RowLoop)
							SELECTEDROW = data->GetRow(data->KeyFromRow(RowLoop));
						else
							Skipping=1;
					}
					else
					if(strcmp(ThisCmd,"ENDLOOP")==0)
					{
						if(SeekBack>=0)
						{
							if (data->GetNumRows()>++RowLoop)
							{
								fseek(f,SeekBack,SEEK_SET);
								SELECTEDROW = data->GetRow(data->KeyFromRow(RowLoop));
							}
							else
								SeekBack=-1;
						}
						if ((Skipping==1)&&(IfState==0))
							Skipping=0;
					}
					else
					if (strcmp(ThisCmd,"REFRESH")==0)
						*data = *backup;
					else
					if (strcmp(ThisCmd,"NEWDB")==0)
					{
						char *NEWDB=ParseQuery(ThisPCmd,"DB");
						delete backup;
						if (ReadDB(&backup,DBmain,NEWDB)>=0)
							*data = *backup;
						free(NEWDB);
					}
					else
					if (strcmp(ThisCmd,"HTML")==0)
					{
						output("<BR>Nesting an 'HTML' request in a PAGE.  Are you INSANE?!<BR>\n");
					}
					else
					if(strcmp(ThisCmd,"IF")==0)
					{
						if(TrueCondition(data,RowLoop,ThisPCmd)==0)
							Skipping = 1;
						IfState=1;
					}
					else
					{
						ParseRequests(data, DBmain, ThisPCmd);
					}
				}
				free(ThisCmd);
				free(ThisPCmd);
				State=0;
			}
			break;
		default:
			output("<BR>Illegal state!!");
			char *c=malltoa(State);
			output(c);
			free(c);
			output("<BR>");
			State = -2;
			break;
	}

	fclose(f);
	delete backup;
	free(FileName);
}


void RawSpitData(DATA *data)
{
	for(long R=0; data->GetRow(data->KeyFromRow(R))!=0; R++)
		for(long C=0; data->GetTag(C)!=0; C++)
		{
			output(data->GetCol(data->KeyFromRow(R),data->GetTag(C)));
			output("<BR>\n");
		}
}


void Field(DATA *data, char *QUERYSTRING)
{
	char **ColTags;
	char **ColPcts;
	char KEYVAL[10];
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);


	if (SELECTEDROW==0)
		output("<BR>No row is selected!<BR>");
	else
	for(long C=0;C<data->GetNumCols();C++)
	{
		if(ColTags[C]!=0)
		{
			if (strcmp(ColTags[C],"DBKEY")==0)
			{
				for(long r=0;r<data->GetNumRows();r++)
					if (SELECTEDROW==data->GetRow(data->KeyFromRow(r)))
						output(myltoa(data->KeyFromRow(r),KEYVAL,10));
			}
			else
			if(strcmp(ColTags[C],"DBNAME")==0)
				output(data->DataName);
			else
			{
				output(SELECTEDROW[data->WhichCol(ColTags[C])]);
			}
		}
	}

	FreeTableCols(&ColTags,&ColPcts,data);
}

void UpdateData(DATA *data)
{
	long err;

	if((err=data->SaveData())!=1)
		switch(err)
		{
			case -1:
				output("<BR><BR>** Update data configuration error.<BR>\n");
				break;
			case -2:
				output("<BR><BR>** Unable to rewrite data file!!!<BR>\n");
				break;
		};
}



void List(DATA *data, char *QUERYSTRING)
{
	long R=0;
	long C=0;
	char Row[10];
	char Key[10];
	char **ColTags;
	char **ColPcts;

	Select(data,QUERYSTRING);
	SortBy(data,QUERYSTRING);

	char *BackCMD = ParseQuery(QUERYSTRING,"REQ");
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);

	if (PageMode==0)
		output("Click row number to view details.<BR><BR>\n");

	output("<TABLE WIDTH=100%>\n<TR><TD WIDTH=5%>Row</TD>");
	for(C=0;C<data->GetNumCols();C++)
		if((ColTags[C]!=0)&&(ColPcts[C]!=0))
		{
			output("<TD WIDTH=");
			output(ColPcts[C]);
			output("%>");
			output(ColTags[C]);
			output("</TD>\n");
		}
	output("</TR>\n");

	// now show the data
	while(data->GetRow(data->KeyFromRow(R))!=0)
	{
		myltoa(data->KeyFromRow(R),Key,10);
		myltoa(R+1,Row,10);
		output("<TR><TD WIDTH=5%><a href=db.exe?REQ=DETAILS&BACKREQ=");
		output(BackCMD);
		output("&DBKEY=");
		output(Key);
		output(OLDQSTRING.buf());
		output(">");
		output(Row);
		output("</a></TD>");
		for(C=0;C<data->GetNumCols();C++)
			if((ColTags[C]!=0)&&(ColPcts[C]!=0))
			{
				output("<TD WIDTH=");
				output(ColPcts[C]);
				output("%>");
				output(data->GetCol(data->KeyFromRow(R),ColTags[C]));
				output("</TD>");
			}
		output("</TR>\n");
		R++;
	}

	output("</TABLE><BR><BR>\n");
	if((strcmp(BackCMD,"LIST")==0)&&(PageMode==0))
	{
		output("<a href=db.exe?REQ=EDIT");
		output(OLDQSTRING.buf());
		output(">Go to data edit mode</A><BR><BR>\n");
		output("<a href=db.exe?REQ=TAGS");
		output(OLDQSTRING.buf());
		output(">Go to tag design mode</a><BR><BR>\n");
	}

	FreeTableCols(&ColTags,&ColPcts,data);
	free(BackCMD);
}

void EditAll(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *Password=ParseQuery(QUERYSTRING,"DBPASS");
	if(strcmp(Password,DBmain->PWORD)!=0)
	{
		output("<BR>This function requires a valid password.<BR>");
		free(Password);
		return;
	}
	else
		free(Password);

	long R=0;
	long C=0;
	char Row[10];
	char Key[10];
	char **ColTags;
	char **ColPcts;

	Select(data,QUERYSTRING);
	SortBy(data,QUERYSTRING);

	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);

	// output table and header row
	output("Click row number to edit.<BR><BR>\n");

	output("<TABLE WIDTH=100%>\n<TR><TD WIDTH=5%>Row</TD>");
	for(C=0;C<data->GetNumCols();C++)
		if((ColTags[C]!=0)&&(ColPcts[C]!=0))
		{
			output("<TD WIDTH=");
			output(ColPcts[C]);
			output("%>");
			output(ColTags[C]);
			output("</TD>\n");
		}
	output("</TR>\n");

	// now show the data
	while(data->GetRow(data->KeyFromRow(R))!=0)
	{
		myltoa(data->KeyFromRow(R),Key,10);
		myltoa(R+1,Row,10);
		output("<TR><TD WIDTH=5%><a href=db.exe?REQ=EDITDETAILS&EDTKEY=");
		output(Key);
		output(OLDQSTRING.buf());
		output(">");
		output(Row);
		output("</a></TD>");
		for(C=0;C<data->GetNumCols();C++)
			if((ColTags[C]!=0)&&(ColPcts[C]!=0))
			{
				output("<TD WIDTH=");
				output(ColPcts[C]);
				output("%>");
				output(data->GetCol(data->KeyFromRow(R),ColTags[C]));
				output("</TD>");
			}
		output("</TR>\n");
		R++;
	}

	output("</TABLE><BR><BR>\n");

	if (PageMode==0)
	{
		output("<a href=db.exe?REQ=ADDDETAILS");
		output(OLDQSTRING.buf());
		output(">Add a new item</A><BR><BR>\n");
		output("<a href=db.exe?REQ=LIST");
		output(OLDQSTRING.buf());
		output(">Go to data list mode</A><BR><BR>\n");
		output("<a href=db.exe?REQ=TAGS");
		output(OLDQSTRING.buf());
		output(">Go to tag design mode</a><BR><BR>\n");
	}

	FreeTableCols(&ColTags,&ColPcts,data);
}

void Details(DATA *data, char *QUERYSTRING)
{
	char **ColTags;
	char **ColPcts;
	char *BackCMD=ParseQuery(QUERYSTRING,"BACKREQ");
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);
	char *MyKey=ParseQuery(QUERYSTRING,"DBKEY");

	if (MyKey[0] != 0)
	{
		long KeyNum = atol(MyKey);
		if (data->GetRow(KeyNum) != 0)
		{
			output("<TABLE WIDTH=100%>\n");
			for(long C=0;C<data->GetNumCols();C++)
			{
				output("<TR><TD WIDTH=20%>");
				output(data->GetTag(C));
				output("</TD>");
				output("<TD WIDTH=80%>");
				output(data->GetCol(KeyNum,data->GetTag(C)));
				output("</TD></TR>\n");
			}
			output("</TABLE>\n");
			if(strcmp(BackCMD,"")==0)
				strcpy(BackCMD,"VIEW");
			if (PageMode==0)
			{
				output("<BR><BR>");
				output("<a href=db.exe?REQ=");
				output(BackCMD);
				output( OLDQSTRING.buf());
				output(">Back to main list</a><BR>\n");
			}
		}
		else
		{
			output("<BR>");
			output(MyKey);
			output(" is not a valid row number<BR>.\n");
		}
	}
	else
		output("<BR>Illegal row number<BR>\n");
	free(BackCMD);
	free(MyKey);
}

void AddDetails(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *Password=ParseQuery(QUERYSTRING,"DBPASS");
	if(strcmp(Password,DBmain->PWORD)!=0)
	{
		output("<BR>This function requires a valid password.<BR>");
		free(Password);
		return;
	}
	else
		free(Password);

	char **ColTags;
	char **ColPcts;
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);
	char MyCol[10];
	char MyMax[10];
	char CurCD[10];
	long C=0;

	output("<FORM ACTION=db.exe METHOD=GET>\n");
	output("<input type=hidden name=REQ value=ADDROW>\n");
	output("<input type=hidden name=DB value=");
	output(data->DataName);
	output(">\n");
	output("<input type=hidden name=DBPASS value=");
	output(DBmain->PWORD);
	output(">\n");
	for(C=0;C<data->GetNumCols();C++)
		if(ColTags[C]!=0)
		{
			CurCD[0] = 'T';
			myltoa(C+1,&CurCD[1],10);
			output("<input type=hidden name=");
			output(CurCD);
			output(" value=");
			output(ColTags[C]);
			output(">\n");
		};
	output("<TABLE WIDTH=100%>\n");
	for(C=0;C<data->GetNumCols();C++)
	{
		MyCol[0] = 'D';
		myltoa(C+1, &MyCol[1], 10);
		myltoa(data->GetTagMax(data->GetTag(C)),MyMax, 10);
		output("<TR><TD WIDTH=20%>");
		output(data->GetTag(C));
		output("</TD>");
		output("<TD WIDTH=80%><input type=text size=");
		output(MyMax);
		output(" name=");
		output(MyCol);
		output(" maxlength=");
		output(MyMax);
		output(" value=\"\">");
		output("</TD></TR>\n");
	}
	output("</TABLE>\n");
	output("<BR><BR><input type=submit value=Submit><BR>\n");
	if (PageMode==0)
	{
		output("<BR><BR>");
		output("<a href=db.exe?REQ=EDIT");
		output( OLDQSTRING.buf());
		output(">Cancel back to main list.</a><BR>\n");
	}
	output("</FORM>\n");
}

void EditDetails(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *Password=ParseQuery(QUERYSTRING,"DBPASS");
	if(strcmp(Password,DBmain->PWORD)!=0)
	{
		output("<BR>This function requires a valid password.<BR>");
		free(Password);
		return;
	}
	else
		free(Password);

	char **ColTags;
	char **ColPcts;
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);
	char *MyKey=ParseQuery(QUERYSTRING,"EDTKEY");
	char MyCol[10];
	char MyMax[10];
	char CurCD[10];
	long C=0;

	if (MyKey[0] != 0)
	{
		long KeyNum = atol(MyKey);
		if (data->GetRow(KeyNum) != 0)
		{
			output("<FORM ACTION=db.exe METHOD=GET>\n");
			output("<input type=hidden name=EDTKEY value=");
			output(MyKey);
			output(">\n");
			output("<input type=hidden name=REQ value=CHANGEROW>\n");
			output("<input type=hidden name=DB value=");
			output(data->DataName);
			output(">\n");
			output("<input type=hidden name=DBPASS value=");
			output(DBmain->PWORD);
			output(">\n");
			for(C=0;C<data->GetNumCols();C++)
				if(ColTags[C]!=0)
				{
					CurCD[0] = 'T';
					myltoa(C+1,&CurCD[1],10);
					output("<input type=hidden name=");
					output(CurCD);
					output(" value=");
					output(ColTags[C]);
					output(">\n");
				};
			output("<TABLE WIDTH=100%>\n");
			for(C=0;C<data->GetNumCols();C++)
			{
				MyCol[0] = 'D';
				myltoa(C+1, &MyCol[1], 10);
				myltoa(data->GetTagMax(data->GetTag(C)),MyMax, 10);
				output("<TR><TD WIDTH=20%>");
				output(data->GetTag(C));
				output("</TD>");
				output("<TD WIDTH=80%><input type=text size=");
				output(MyMax);
				output(" name=");
				output(MyCol);
				output(" maxlength=");
				output(MyMax);
				output(" value=\"");
				output(data->GetCol(KeyNum,data->GetTag(C)));
				output( "\">");
				output("</TD></TR>\n");
			}
			output("</TABLE>\n");
			output("<BR><BR><input type=submit value=Submit><BR>\n");
			output("<BR><BR>");
			output("<a href=db.exe?REQ=DELROW&EDTKEY=");
			output(MyKey);
			output( OLDQSTRING.buf());
			output(">Delete this row.</a><BR>\n");
			if (PageMode==0)
			{
				output("<BR><BR>");
				output("<a href=db.exe?REQ=EDIT");
				output( OLDQSTRING.buf());
				output(">Cancel back to main list.</a><BR>\n");
			}
			output("</FORM>\n");
		}
		else
		{
			output("<BR>");
			output(MyKey);
			output(" is not a valid row number<BR>.\n");
		}
	}
	else
		output("<BR>Illegal row number<BR>\n");
	free(MyKey);
}

int ModifyRow(DATA *data, char *QUERYSTRING, long KeyNum)
{
	long C=0;
	char MyCol[10];
	char *NewVal;

	for(C=0;C<data->GetNumCols();C++)
	{
		MyCol[0]='D';
		myltoa(C+1, &MyCol[1], 10);
		NewVal = ParseQuery(QUERYSTRING,MyCol);
		if(data->AddCol(KeyNum,data->GetTag(C),NewVal)==0)
		{
			output("<BR><BR>Error changing row ");
			char *c=malltoa(KeyNum);
			output(c);
			free(c);
			output(", column tag ");
			output(data->GetTag(C));
			output("<BR>\n");
			return 0;
		}
		free(NewVal);
	}
	UpdateData(data);
	return 1;
}

void ChangeRow(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *Password=ParseQuery(QUERYSTRING,"DBPASS");
	if(strcmp(Password,DBmain->PWORD)!=0)
	{
		output("<BR>This function requires a valid password.<BR>");
		free(Password);
		return;
	}
	else
		free(Password);

	char **ColTags;
	char **ColPcts;
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);
	char *MyKey=ParseQuery(QUERYSTRING,"EDTKEY");

	if (MyKey[0] != 0)
	{
		long KeyNum = atol(MyKey);
		if (data->GetRow(KeyNum)!= 0)
		{
			if (ModifyRow(data,QUERYSTRING,KeyNum)==0) return;
			if (PageMode==0)
				EditAll(data,DBmain,QUERYSTRING);
		}
		else
		{
			output("<BR>");
			output(MyKey);
			output(" is not a valid row number<BR>.\n");
		}
	}
	else
		output("<BR>Illegal row number<BR>\n");
	free(MyKey);
}

void AddRow(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *Password=ParseQuery(QUERYSTRING,"DBPASS");
	if(strcmp(Password,DBmain->PWORD)!=0)
	{
		output("<BR>This function requires a valid password.<BR>");
		free(Password);
		return;
	}
	else
		free(Password);

	char **ColTags;
	char **ColPcts;
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);
	long NewRowKey;

	NewRowKey = data->GetNumRows();
	data->AddRow();
	if (ModifyRow(data,QUERYSTRING,NewRowKey)==0) return;
	if (PageMode==0)
		EditAll(data,DBmain,QUERYSTRING);
}


void DelRow(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *Password=ParseQuery(QUERYSTRING,"DBPASS");
	if(strcmp(Password,DBmain->PWORD)!=0)
	{
		output("<BR>This function requires a valid password.<BR>");
		free(Password);
		return;
	}
	else
		free(Password);

	char **ColTags;
	char **ColPcts;
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);
	char *MyKey = ParseQuery(QUERYSTRING,"EDTKEY");

	if (MyKey[0] != 0)
	{
		long KeyNum = atol(MyKey);
		if (data->GetRow(KeyNum) != 0)
		{
			data->DelRow(KeyNum);
			UpdateData(data);
			if (PageMode==0)
				EditAll(data,DBmain,QUERYSTRING);
		}
		else
		{
			output("<BR>");
			output(MyKey);
			output(" is not a valid row number<BR>.\n");
		}
	}
	else
		output("<BR>Illegal row number<BR>\n");
	free(MyKey);
}

const char *ABCs="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`~1!2@3#4$5%6^7&8*9(0)-_=+[{]}|;:',<.>/? ";
const char *FILTER="howmuchwoodwouldawoodchuckchuckifawoodchuckcouldchuckwoodhesellsseashellsattheseashore";

long ReadINI(DBTYPE *DBmain, char* pospass)
{
	char  NextCol[256];
	long C = 0;
	FILE *f;
	char *Path="cgi-bin/db.ini";
	long l;

	DBmain->NumTables=0;

	f=fopen("cgi-bin/db.ini","r");
	if(f<=0)
	{
		f=fopen("db.ini","r");
		if(f<=0)
			return -2;
	}

	if (!feof(f))
	{
		if(readLine(f,NextCol)>=0)
		{
			if(NextCol[0]==0)
			{
				if(pospass[0]==0)
					strcpy(DBmain->PWORD,"");
				else
				{
					char *ENCRYPTED=Encrypt(pospass);
					output("<BR><BR>New password is: ");
					output(ENCRYPTED);
					output("\n\n<P><BR>\n\n");
					free(ENCRYPTED);
					return -1;
				}
			}
			else
			{
				for(l=0;NextCol[l]!=0;l++);
				if(NextCol[l-1]==10)
					NextCol[l-1]=0;
				char *DECRYPTED = Decrypt(NextCol);
				strcpy(DBmain->PWORD,DECRYPTED);
				free(DECRYPTED);
			}
		}
	}
	while ((!feof(f))&&(DBmain->NumTables<20))
	{
		if(readLine(f,NextCol)<0) break;
		for(l=0;NextCol[l]!=0;l++);
		if(NextCol[l-1]==10)
			NextCol[l-1]=0;
		strcpy(DBmain->TableNames[DBmain->NumTables],NextCol);
		if (!feof(f))
		{
			if(readLine(f,NextCol)<0) break;
			for(l=0;NextCol[l]!=0;l++);
			if(NextCol[l-1]==10)
				NextCol[l-1]=0;
			strcpy(DBmain->StructPaths[DBmain->NumTables],NextCol);
			if (!feof(f))
			{
				if(readLine(f,NextCol)<0) break;
				for(l=0;NextCol[l]!=0;l++);
				if(NextCol[l-1]==10)
					NextCol[l-1]=0;
				strcpy(DBmain->DataPaths[DBmain->NumTables],NextCol);
				DBmain->NumTables++;
			}
		}
	}
	fclose(f);
	return 1;
}

void Setup(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{};
void SetupUpdate(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{};
void InitializeSystem()
{};


long ReadDB(DATA **data, DBTYPE *DBmain, char *DBNAME)
{
	long DBindex = -1;
	long err=1;

	if(DBNAME[0]!=0)
	{
		for(long i=0;i<DBmain->NumTables;i++)
			if(strcmp(DBmain->TableNames[i],DBNAME)==0)
				DBindex=i;
	}

	if(DBindex<0)
	{
		output("<BR><BR>**A valid 'DB=' parameter is required!<BR>\n");
		return -1;
	}

	*data = new DATA(DBmain->TableNames[DBindex],DBmain->StructPaths[DBindex],DBmain->DataPaths[DBindex]);

	if((err=(*data)->LoadTags())!=1)
	{
		switch(err)
		{
			case -1:
				output("<BR><BR>** Read tags configuration error.<BR>\n");
				break;
			case -2:
				output("<BR><BR>** Unable to open tags file.<BR>\n");
				break;
			case -3:
				output("<BR><BR>** Unable to read tags file.<BR>\n");
				break;
		};
	}
	else
	if((err=(*data)->LoadData())!=1)
	{
		switch(err)
		{
			case -1:
				output("<BR><BR>** Read data configuration error.<BR>\n");
				break;
			case -2:
				output("<BR><BR>** Unable to open data file.<BR>\n");
				break;
			case -3:
				output("<BR><BR>** Unable to read data file.<BR>\n");
				break;
		};
	}


	if(err!=1)
		return -1;

	return DBindex;
}


void ParseRequests(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *REQ = ParseQuery(QUERYSTRING,"REQ");

	if (strcmp(REQ,"LIST")==0)
			List(data, QUERYSTRING);
	else
	if (strcmp(REQ,"VIEW")==0)
			List(data, QUERYSTRING);
	else
	if (strcmp(REQ,"EDIT")==0)
			EditAll(data, DBmain, QUERYSTRING);
	else
	if (strcmp(REQ,"DETAILS")==0)
			Details(data, QUERYSTRING);
	else
	if (strcmp(REQ,"EDITDETAILS")==0)
			EditDetails(data, DBmain, QUERYSTRING);
	else
	if (strcmp(REQ,"ADDDETAILS")==0)
			AddDetails(data, DBmain, QUERYSTRING);
	else
	if (strcmp(REQ,"CHANGEROW")==0)
			ChangeRow(data, DBmain, QUERYSTRING);
	else
	if (strcmp(REQ,"ADDROW")==0)
			AddRow(data, DBmain, QUERYSTRING);
	else
	if (strcmp(REQ,"DELROW")==0)
			DelRow(data, DBmain, QUERYSTRING);
	else
	if (strcmp(REQ,"TAGS")==0)
			TagDetails(data, DBmain, QUERYSTRING);
	else
	if (strcmp(REQ,"DELTAG")==0)
			DelTag(data, DBmain, QUERYSTRING);
	else
	if (strcmp(REQ,"MODIFY")==0)
			EditTags(data, DBmain, QUERYSTRING);
	else
	if (strcmp(REQ,"SORT")==0)
			SortBy(data, QUERYSTRING);
	else
	if (strcmp(REQ,"SELECT")==0)
			Select(data, QUERYSTRING);
	else
	if (strcmp(REQ,"FIELD")==0)
			Field(data, QUERYSTRING);
	else
	if (strcmp(REQ,"HTML")==0)
			DoPage(data, DBmain, QUERYSTRING);
	else
	{
		output("'");
		output(REQ);
		output("' is not a legal request.<BR>\n");
	}
	free(REQ);
}
char ABCeq(char C)
{
	for(unsigned long A=0;A<strlen(ABCs);A++)
		if(C==ABCs[A]) return ABCs[A];
	return '.';
}

long ABCindex(char C)
{
	for(unsigned long A=0;A<strlen(ABCs);A++)
		if(C==ABCs[A]) return A;
	return 0;
}

char *Encrypt(char *ENCRYPTME)
{
	char *INTOME;

	INTOME = (char *)malloc(sizeof(char) * strlen(ENCRYPTME) + 1);
	for(long S=0, D=0; ENCRYPTME[S]!=0;S++)
	{
		INTOME[S] = ABCeq(ENCRYPTME[S]);
		if (INTOME[S]==(char)0)
			INTOME[S]=ENCRYPTME[S];
		else
		for(unsigned long F=S;F<(unsigned long)strlen(FILTER);F+=(long)strlen(ENCRYPTME))
		{
			long X = ABCindex(INTOME[S]);
			X = X + ABCindex(FILTER[F]);
			if (X>=(long)strlen(ABCs))
				X = X-(long)strlen(ABCs);
			INTOME[S]=ABCs[X];
		}
		INTOME[S+1]=0;
	}
	return INTOME;
}

char *Decrypt(char *DECRYPTME)
{
	char *INTOME;

	INTOME = (char *)malloc(sizeof(char) * strlen(DECRYPTME) + 1);
	for(long S=0, D=0; DECRYPTME[S]!=0;S++)
	{
		INTOME[S] = ABCeq(DECRYPTME[S]);
		if (INTOME[S]==(char)0)
			INTOME[S]=DECRYPTME[S];
		else
		for(unsigned long F=S;F<(unsigned long)strlen(FILTER);F+=(unsigned long)strlen(DECRYPTME))
		{
			long X = ABCindex(INTOME[S]);
			X = X - ABCindex(FILTER[F]);
			if (X<0)
				X = X+(unsigned long)strlen(ABCs);
			INTOME[S]=ABCs[X];
		}
		INTOME[S+1]=0;
	}
	return INTOME;
}


long BuildTableCols(char ***CTags, char ***CPcts, DATA *data, char *QUERYSTRING)
{
	long C=0;
	long TotWidth=0;
	char CurCD[10];
	char *CurTag;
	char WidthPct[10];
	double WidthF, f1, f2;
	int I1, I2;
	char **ColTags;
	char **ColPcts;

	ColTags = (char **)malloc(sizeof(char *) * (data->GetNumCols()+1));
	ColPcts = (char **)malloc(sizeof(char *) * (data->GetNumCols()+1));

	CurTag=ParseQuery(QUERYSTRING,"DB");
	if (CurTag[0]!=0)
	{
		OLDQSTRING=OLDQSTRING+"&DB=";
		OLDQSTRING=OLDQSTRING+CurTag;
	}
	free(CurTag);

	CurTag=ParseQuery(QUERYSTRING,"DBPASS");
	if (CurTag[0]!=0)
	{
		OLDQSTRING=OLDQSTRING + "&DBPASS=";
		OLDQSTRING=OLDQSTRING + CurTag;
	}
	free(CurTag);

	// fetch all tag column values from T1, T2, T3...T99
	for(C=0;C<data->GetNumCols();C++)
	{
		ColTags[C]=0;
		ColPcts[C]=0;

		CurCD[0] = 'T';
		myltoa(C+1,&CurCD[1],10);
		CurTag=ParseQuery(QUERYSTRING, CurCD);
		if(CurTag[0]!=0)
		{
			OLDQSTRING=OLDQSTRING + "&";
			OLDQSTRING=OLDQSTRING + CurCD;
			OLDQSTRING=OLDQSTRING + "=";
			OLDQSTRING=OLDQSTRING + CurTag;
			ColTags[C] = (char *)malloc(sizeof(char)*(1+strlen(CurTag)));
			strcpy(ColTags[C],CurTag);
			TotWidth+=data->GetTagMax(CurTag);
		}
		free(CurTag);
		CurTag=ParseQuery(QUERYSTRING,data->GetTag(C));
		if (CurTag[0]!=0)
		{
			OLDQSTRING=OLDQSTRING + "&";
			OLDQSTRING=OLDQSTRING+ data->GetTag(C);
			OLDQSTRING=OLDQSTRING+"=";
			OLDQSTRING=OLDQSTRING+ CurTag;
		}
		free(CurTag);
	}
	CurTag=ParseQuery(QUERYSTRING,"SORTBY");
	if (CurTag[0]!=0)
	{
		OLDQSTRING=OLDQSTRING + "&SORTBY=";
		OLDQSTRING=OLDQSTRING + CurTag;
	}
	free(CurTag);
	CurTag=ParseQuery(QUERYSTRING,"ANDTHENBY");
	if (CurTag[0]!=0)
	{
		OLDQSTRING=OLDQSTRING + "&ANDTHENBY=";
		OLDQSTRING=OLDQSTRING + CurTag;
	}
	free(CurTag);


	// now build percentages table
	WidthF = TotWidth * .05;	// take 5% away for the row column
	TotWidth -= (long)WidthF;

	for(C=0;C<data->GetNumCols();C++)
	{
		if(ColTags[C]!=0)
		{
			if (data->WhichCol(ColTags[C])>=0)
			{
				f1 = (double)data->GetTagMax(ColTags[C]);
				f2 = (double)TotWidth;
				WidthF = f1/f2;
				strcpy(WidthPct,fcvt( WidthF, 2,  &I1, &I2 ));
				ColPcts[C] = (char *)malloc(sizeof(char) * (1+strlen(WidthPct)));
				strcpy(ColPcts[C],WidthPct);
			}
		}
	}

	*CTags = ColTags;
	*CPcts = ColPcts;
	return TotWidth;
}

void FreeTableCols(char ***ColTags, char ***ColPcts, DATA *data)
{
	long upto = data->GetNumCols();

	for(long C=0;C<upto;C++)
	{
		if((*ColTags)[C]!=0)
		{
			free((*ColTags)[C]);
			free((*ColPcts)[C]);
		};
	}

	free(*ColTags);
	(*ColTags)=0;
	free(*ColPcts);
	(*ColPcts)=0;
}

    string::string(char *copy)          // copy constructor
    {
      s = new char[strlen(copy)+1];
      strcpy(s,copy);
    };

    string::string(string& copy)        // copy constructor for other strs
    {
      s = new char[strlen(copy.s)+1];
      strcpy(s,copy.s);
    };

    string::string(int size)            // sized string constructor
    {
      if (size > 0 && size < 255)
	s = new char[size+1];
    };

    string::string()                    // void empty string constructor
    {
      s = new char[1];
      s[0] = '\0';
    };

    void string::init(int size)	        // alternate constructor
    {
      if (s == NULL)
	delete s;
      if (size >= 0 && size < 255)
	s = new char[size+1];
    };

    void string::init(char *copy)	// alternate copy constructor
    {
      if (s == NULL)
	delete s;
      s = new char[strlen(copy)+1];
      strcpy(s,copy);
    };

    string::~string()                   // destructor
    {
      delete s;
    };

    char& string::operator [](int i)    // implements character references
    {
      if ((i < 0) || (i > (int)strlen(s)))
	return s[0];
      return(s[i]);
    };

    string& string::operator =(char *copy)
    {                                   // assignment to character arrays
      if (s != NULL)
	delete s;
      s = new char[strlen(copy)+1];
      strcpy(s,copy);
      return (*this);
    };

    string& string::operator =(string& copy)
    {                                   // assignment to other strings
      if (s != NULL)
	delete s;
      s = new char[strlen(copy.s)+1];
      strcpy(s,copy.s);
      return (*this);
    };

    char *string::operator +(char *s2)  // add a string to a char array
    {
      char *temp;

      temp = new char[strlen(s)+strlen(s2)+1];
      strcpy(temp,s);
      strcpy(strchr(temp,0),s2);
      return (temp);
    };

    char *string::operator +(char c)  // add a string to a char array
    {
      char *temp;
	  char *cbuf=new char[2];
	  cbuf[0]=c;
	  cbuf[1]=0;

      temp = new char[strlen(s)+2];
      strcpy(temp,s);
      strcpy(strchr(temp,0),cbuf);
      return (temp);
    };

    char *string::operator +(string& s2)// add a string to a string
    {
      char *temp;

      temp = new char[strlen(s)+strlen(s2.s)+1];
      strcpy(temp,s);
      strcpy(strchr(temp,0),s2.s);
      return (temp);
    };


    int string::len()                   // get the length
    {
      if (s == NULL)
    	return 0;
      return (int)strlen(s);
    };

    char *string::buf()                   // get the buffer
    {
      if (s == NULL)
			return 0;
      return (s);
    };

    char *string::operator *()           // if you need func.s from string.h
    {
      return s;
    };

    int string::operator ==(char *s2)    // string equal to char array
    {
      if (strcmp(s,s2) == 0)
	return 1;
      else
	return 0;
    };

    int string::operator ==(string& s2)  // string equal to string
    {
      if (strcmp(s,s2.s) == 0)
	return 1;
      else
	return 0;
    };

    int string::operator !=(char *s2)    // string not equal to char array
    {
      if (strcmp(s,s2) == 0)
	return 0;
      else
	return 1;
    };

    int string::operator !=(string& s2)  // string not equal to string
    {
      if (strcmp(s,s2.s) == 0)
	return 0;
      else
	return 1;
    };

    int string::operator >(char *s2)     // string greater than char array
    {
      if (strcmp(s,s2) > 0)
	return 1;
      else
	return 0;
    };

    int string::operator >(string& s2)   // string greater than string
    {
      if (strcmp(s,s2.s) > 0)
	return 1;
      else
	return 0;
    };

    int string::operator <(char *s2)     // string less than char array
    {
      if (strcmp(s,s2) < 0)
	return 1;
      else
	return 0;
    };

    int string::operator <(string& s2)   // string less than string
    {
      if (strcmp(s,s2.s) < 0)
	return 1;
      else
	return 0;
    };

    int string::operator >=(char *s2)    // string greater/equal char array
    {
      int i;

      if (((i = strcmp(s,s2)) == 0) || (i > 0))
	return 1;
      else
	return 0;
    };

    int string::operator >=(string& s2)  // string greater/equal string
    {
      int i;

      if (((i = strcmp(s,s2.s)) == 0) || (i > 0))
	return 1;
      else
	return 0;
    };

    int string::operator <=(char *s2)    // string less than char array
    {
      int i;

      if (((i = strcmp(s,s2)) == 0) || (i < 0))
	return 1;
      else
	return 0;
    };

    int string::operator <=(string& s2)  // string less/equal to string
    {
      int i;

      if (((i = strcmp(s,s2.s)) == 0) || (i < 0))
	return 1;
      else
	return 0;
    };

    char *string::operator %(string& s2)// return string position
    {
      char *p;


      p = strstr(s,s2.s);
      return p;
    };

    char *string::operator %(char *s2)  // return char array position
    {
      char *p;


      p = strstr(s,s2);
      return p;
    };

    char *string::operator -(int x)	 // remove x chars from front
    {
      char *t;

      if (x <= (int)strlen(s) && x > 0)
      {
	 t = new char[strlen(s)-x+1];
	 strcpy(t,&s[x-1]);
	 return t;
      };
      return s;
    };


    char *string::operator -(string& s2)// remove s2 from string
    {
      return s2.s;
    };

    char *string::operator -(char *s2) 	// remove char s2 from string
    {
      return s2;
    };

	int string::Find(char c)
	{
		for(int x=0;x<(int)strlen(s);x++)
			if(s[x]==c) return x;
		return -1;
	};

	int string::Find(char *c)
	{
		if(strlen(c)>strlen(s))
			return -1;
		for(int x=0;x<(int)strlen(s)-(int)strlen(c);x++)
		{
			if(s[x]==c[0])
			{
				int flag=0;
				for(int x1=1;x1<(int)strlen(c);x1++)
				{
					if(s[x+x1]!=c[x1])
					{
						flag=1;
						break;
					}
				}
				if(flag==0)
					return x;
			}
		}
		return -1;
	};

	char *string::Left(int numchars)
	{
		if(numchars>=(int)strlen(s))
			return s;
		char *buf=new char[numchars+2];
		strncpy(buf,s,numchars);
		buf[numchars]=0;
		return buf;
	};

	char *string::Right(int numchars)
	{
		if(numchars>=(int)strlen(s))
			return s;
		int index=(int)strlen(s)-numchars;
		char *buf=new char[numchars+1];
		strncpy(buf,&s[index],numchars);
		buf[numchars]=0;
		return buf;
	};

	char *string::Mid(int startchar)
	{
		if(startchar>=(int)strlen(s))
		{
			char *buf=new char[1];
			buf[0]=0;
			return buf;
		}
		char *buf=new char[strlen(s)-startchar+1];
		strncpy(buf,&s[startchar],strlen(s)-startchar);
		buf[strlen(s)-startchar]=0;
		return buf;
	};

    std::ostream& operator <<(std::ostream& o, string& s2)    // stream out
    {
      printf("%s",s2.s);
      return o;
    };

    std::istream& operator >>(std::istream& o, string& s2)    // stream in
    {
      return o;
    };


void TagDetails(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *Password=ParseQuery(QUERYSTRING,"DBPASS");
	if(strcmp(Password,DBmain->PWORD)!=0)
	{
		output("<BR>This function requires a valid password.<BR>");
		free(Password);
		return;
	}
	else
		free(Password);

	long R=0;
	long C=0;
	char **ColTags;
	char **ColPcts;
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);
	char MyCol[10];
	char MyMax[10];
	char CurCD[10];

	// output table and header row
	output("Click submit for changes, or click a hyperlink.<BR><BR>\n");

	output("<FORM ACTION=db.exe METHOD=GET>\n");
	output("<input type=hidden name=REQ value=MODIFY>\n");
	output("<input type=hidden name=DB value=");
	output(data->DataName);
	output(">\n");
	output("<input type=hidden name=DBPASS value=");
	output(DBmain->PWORD);
	output(">\n");
	for(C=0;C<data->GetNumCols();C++)
		if(ColTags[C]!=0)
		{
			CurCD[0]='T';
			myltoa(C+1,&CurCD[1],10);
			output("<input type=hidden name=");
			output(CurCD);
			output(" value=");
			output(ColTags[C]);
			output(">\n");
		};
	output("<TABLE WIDTH=100%>\n");
	for(C=0;C<data->GetNumCols();C++)
	{
		myltoa(C+1, MyCol, 10);
		myltoa(data->GetTagMax(data->GetTag(C)),MyMax, 10);
		output("<TR><TD WIDTH=60%>Name: <input type=text size=50 maxlength=255 name=D");
		output(MyCol);
		output(" value=\"");
		output(data->GetTag(C));
		output( "\">");
		output("</TD>");
		output("<TD WIDTH=20%>Max Length: <input type=text size=5 maxlength=9 name=M");
		output(MyCol);
		output(" value=\"");
			char *c=malltoa(data->GetTagMax(data->GetTag(C)));
			output(c);
			free(c);
		output( "\">");
		output("</TD>");
		output("<TD WIDTH=20%><a href=db.exe?REQ=DELTAG&Tag=");
		output(MyCol);
		output(OLDQSTRING.buf());
		output(">Delete</a></TD></TR>\n");
	}
	output("</TABLE><BR><BR>\n");
	output("Fill in this row and press Submit to add new tag.\n");
	output("<TABLE WIDTH=100%>\n");
	myltoa(data->GetNumCols()+1, MyCol, 10);
	output("<TR><TD WIDTH=60%>Name: <input type=text size=50 maxlength=255 name=D");
	output(MyCol);
	output(" value=\"\">");
	output("</TD>");
	output("<TD WIDTH=20%>Max Length: <input type=text size=5 maxlength=9 name=M");
	output(MyCol);
	output(" value=\"\">");
	output("</TD>");
	output("<TD WIDTH=20%></TD></TR>\n");
	output("</TABLE>\n");

	output("<BR><BR><input type=submit value=Submit><BR><BR>\n");


	output("<a href=db.exe?REQ=LIST");
	output(OLDQSTRING.buf());
	output(">Go to data list mode</A><BR><BR>\n");
	output("<a href=db.exe?REQ=EDIT");
	output(OLDQSTRING.buf());
	output(">Go to data edit mode</A><BR><BR>\n");


	FreeTableCols(&ColTags,&ColPcts,data);

}

void DelTag(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *Password=ParseQuery(QUERYSTRING,"DBPASS");
	if(strcmp(Password,DBmain->PWORD)!=0)
	{
		output("<BR>This function requires a valid password.<BR>");
		free(Password);
		return;
	}
	else
		free(Password);

	char **ColTags;
	char **ColPcts;
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);
	char *MyTag=ParseQuery(QUERYSTRING,"Tag");

	if (MyTag[0] != 0)
	{
		long TagNum = atol(MyTag);
		TagNum--;
		if ((TagNum >= 0)&&(TagNum<data->GetNumCols()))
		{
			data->DelTag(data->GetTag(TagNum));
			UpdateData(data);
			UpdateTags(data);
			if (PageMode==0)
				TagDetails(data,DBmain,QUERYSTRING);
		}
		else
		{
			output("<BR>");
			output(MyTag);
			output(" is not a valid tag number<BR>.\n");
		}
	}
	else
		output("<BR>Illegal tag number<BR>\n");
	free(MyTag);
}

void EditTags(DATA *data, struct DBTYPE *DBmain, char *QUERYSTRING)
{
	char *Password=ParseQuery(QUERYSTRING,"DBPASS");
	if(strcmp(Password,DBmain->PWORD)!=0)
	{
		output("<BR>This function requires a valid password.<BR>");
		free(Password);
		return;
	}
	else
		free(Password);

	long C=0;
	char MyCol[10];
	char MyMax[10];
	long lNewMax;
	char *NewMax;
	char *NewVal;
	char **ColTags;
	char **ColPcts;
	long TotWidth = BuildTableCols(&ColTags, &ColPcts, data, QUERYSTRING);

	for(C=0;C<data->GetNumCols();C++)
	{
		MyCol[0]= 'D';
		myltoa(C+1, &MyCol[1], 10);
		NewVal = ParseQuery(QUERYSTRING,MyCol);
		MyMax[0]= 'M';
		myltoa(C+1, &MyMax[1], 10);
		NewMax = ParseQuery(QUERYSTRING,MyMax);
		lNewMax = atol(NewMax);
		free(NewMax);
		if(data->ModifyTag(data->GetTag(C), NewVal, lNewMax)==0)
		{
			output("<BR><BR>Error changing tag ");
			char *c=malltoa(C);
			output(c);
			free(c);
			output("<BR>\n");
			return;
		}
		free(NewVal);
	}
	MyCol[0]= 'D';
	myltoa(data->GetNumCols()+1, &MyCol[1], 10);
	NewVal = ParseQuery(QUERYSTRING,MyCol);
	MyMax[0]= 'M';
	myltoa(data->GetNumCols()+1, &MyMax[1], 10);
	NewMax = ParseQuery(QUERYSTRING,MyMax);
	lNewMax = atol(NewMax);
	if((NewVal[0]!=0)&&(lNewMax>0))
	{
		if(data->AddTag(NewVal, lNewMax)==0)
			output("<BR><BR>Error adding tag.<BR>\n");
	}
	else
	if((NewVal[0]!=0)||(NewMax[0]!=0))
		output("<BR><BR>New tag is invalid.<BR>\n");

	free(NewMax);
	free(NewVal);

	UpdateData(data);
	UpdateTags(data);
	if (PageMode==0)
		TagDetails(data,DBmain,QUERYSTRING);
}

void UpdateTags(DATA *data)
{
	long err;

	if((err=data->SaveTags())!=1)
		switch(err)
		{
			case -1:
				output("<BR><BR>** Update tags configuration error.<BR>\n");
				break;
			case -2:
				output("<BR><BR>** Unable to rewrite tags file!!!<BR>\n");
				break;
		};
}


void RawSpitTags(DATA *data)
{
	for(long C=0; data->GetTag(C)!=0; C++)
	{
		output(data->GetTag(C));
		output("<BR>\n");
	}
}

int main(int argn, char **argv, char **envp)
{
	if(fout>=0)
		out=fopen("output.txt","w");

#ifdef _WIN32
    HANDLE lockFile = 
       CreateFile(TEXT("cgidb.lock"), // open Two.txt
                  GENERIC_WRITE,         // open for writing
                  0,     
                  NULL,                     // no security
                  OPEN_ALWAYS,              // open or create
                  FILE_ATTRIBUTE_NORMAL,    // normal file
                  NULL); 
#else
    int lockFile = open("cgidb.lock",O_RDWR | O_CREAT, 0666);
    lockf(lockFile,F_LOCK, 0);
#endif

	output("Content-type: text/html\n\n");

	#if defined(DEBUG)
		char *QUERYSTRING = "REQ=HTML&DBKEY=54&DB=cbmcomputers&INPUTFILE=details.html";
	//char *QUERYSTRING = "REQ=LIST&DB=sample&DBPASS=krebnatz&T1=THUMBNAIL";
	#else
	char *QUERYSTRING = getenv("QUERY_STRING");
	#endif

	struct DBTYPE DBmain;
	char *DelMeReq = ParseQuery(QUERYSTRING,"REQ");
	char *PotPass = ParseQuery(QUERYSTRING,"DBPASS");
	char *DelMeDB = ParseQuery(QUERYSTRING,"DB");

	long DBindex=-1;
	DATA *DefData;

	if(strcmp(DelMeReq,"HTML")!=0)
		output("<HTML><BODY>\n");

	if(ReadINI(&DBmain,PotPass)!=1)
	{
		output("<BR><BR>** Unable to read db.ini file!<BR>\n");
		DBindex = -1;
	}
	else
	{
		DBindex = ReadDB(&DefData,&DBmain,DelMeDB);
		if(DBindex>=0)
			ParseRequests(DefData, &DBmain, QUERYSTRING);

		delete DefData;
	}

	if(strcmp(DelMeReq,"HTML")!=0)
		output("</HTML></BODY>\n");

	if(fout>=0)
		fclose(out);

	free(DelMeReq);
	free(PotPass);
	free(DelMeDB);

#ifdef _WIN32
    CloseHandle(lockFile);
#else
    close(lockFile);
#endif
printf("8\n");
	return DBindex;
}
