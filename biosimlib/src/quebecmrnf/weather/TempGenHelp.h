//***************************************************************************
// File:	   TempGenHelp.h
//
// Class:      CTempGenHelp
//
// Summary:    Help class to use TemgGenLib.dll
//
// Description: This class is used to wrap TemgGenLib.dll fonctions into a c++ class
//
// Attributes:   void * m_hDll: handle on the dll
//
// Note:        
//***************************************************************************
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "ERMsg.h"

#define SS_ANSI
#include "stdString.h"
typedef std::vector<CStdString> CStdStringVector;


class CTempGenHelp  
{
public:

    //Correction for exposition
    enum TAlbedo
    { 
        NONE,           //no Albedo correction
        CONIFER_CANOPY  //Correction for conifer canopy
    };

    //Simulated variables in TempGen
    enum TVariable
    {
        TMIN,	//Minimum Temperature (C°)
        TMAX,	//Maximum Temperature (C°)
        PRCP,	//Precipitation (mm)
		TDEW,	//Dew point  (C°)
		RELH,	//Relative humidity (%)
		WNDS	//Wind speed (km/h)
    };

    //type of statistic
    enum TStat
    { 
        LOWEST,     //minimum value of the data
        MEAN,       //mean of the data
        SUM,        //sum of the data
        SUM2,       //sum of square of each data
        STD_DEV,    //standard deviation of the data
		STD_ERR,	//Standar error
        COEF_VAR,   //coeficiant of variation of the data
        VARIANCE,   //variance of the data
        HIGHEST,    //higest value of the data
		QUADRATIC_MEAN, 
        TSS,        //total sum of square
        NB_VALUE,   //number of values added in the statistic
        NB_STAT_TYPE
    };
	

	
	CTempGenHelp();
	virtual ~CTempGenHelp();

    bool IsInit()const;
    ERMsg Initialize( const char* DLLPath = "TempGenLib.dll");

	//initialisation of the simulator
	ERMsg SetNormalDBFilePath( const char* fileName );
	ERMsg SetDailyDBFilePath( const char* fileName);
	void SetTarget( const char* name, const double& lat, const double& lon, float elev, float slope=0, float orientation=0);
	void SetReplication( short nbRep, bool bFixedSeed=false  );
	void SetTGInput(short firstYear, short lastYear, 
					short nbNormalStation, short nbDailyStation, 
					short albedoType, const char* category);

	//execution of the simulator
	ERMsg Generate(void);
	ERMsg Save(const CStdStringVector& outputFilePathVector)const;
	float GetValue(short r, short y, short jd, short v)const;
	double GetAllYearsStat(short var, short dailyStatType, short annualStatType)const;
	double GetGrowingSeasonStat(short var, short dailyStatType, short annualStatType)const;

private:

	void * m_hDll;
    
    typedef bool (*SetNormalDBFilePathF)( const char* fileName, char messageOut[1024] );
	typedef bool (*SetDailyDBFilePathF)( const char* fileName, char messageOut[1024] );
    typedef void (*SetTargetF)( const char* name, const double& lat, const double& lon, float elev, float slope, float orientation);
	typedef void (*SetReplicationF)( short nbRep, bool bFixedSeed );
	typedef void (*SetTGInputF)(short year, short nbYear, 
										short nbNormalStation, short nbDailyStation, 
										short albedoType, const char* cat);
	typedef bool (*GenerateF)(char messageOut[1024]);
	typedef bool (*SaveF)(char** outputFilePathArray, char messageOut[1024] );
	typedef float (*GetValueF)(short r, short y, short jd, short v);
	typedef double (*GetAllYearsStatF)(short var, short dailyStatType, short annualStatType);
	typedef double (*GetGrowingSeasonStatF)(short var, short dailyStatType, short annualStatType);

    SetNormalDBFilePathF m_SetNormalDBFilePath;
    SetDailyDBFilePathF m_SetDailyDBFilePath;
	SetTargetF m_SetTarget;
	SetReplicationF m_SetReplication;
	SetTGInputF m_SetTGInput;
	GenerateF m_Generate;
	SaveF m_Save;
	GetValueF m_GetValue;
	GetAllYearsStatF m_GetAllYearsStat;
	GetGrowingSeasonStatF m_GetGrowingSeasonStat;
};
