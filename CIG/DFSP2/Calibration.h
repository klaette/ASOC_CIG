void FindIP(string &ip)
{
	WORD v = MAKEWORD(1, 1);
	WSADATA wsaData;
	WSAStartup(v, &wsaData); // ¼ÓÔØÌ×½Ó×Ö¿â

	struct hostent *phostinfo = gethostbyname("");
	char *p = inet_ntoa(*((struct in_addr *)(*phostinfo->h_addr_list)));
	int xx = sizeof(p);
	ip = p;
	WSACleanup();
}

//IGM_Calibration
int IGM_Test(int No)               //
{
	int PlenArray[] = { 1, 3, 5, 7 };          //   
	int NoimprovementArray[] = { 3, 5, 7, 10, 15 };       //
	int PlenWayArray[] = { 0, 1, 2, 3 };

	int Temp = No;
	int Plen = PlenArray[Temp / (5 * 4)];       //
	Temp = Temp % (5 * 4);                         //
	int Noimprovement = NoimprovementArray[Temp / 4];                //
	Temp = Temp % 4;
	int PlenWay = PlenWayArray[Temp];

	const int TotaltestIns = 20, Reps = 10;
	vector<vector<int>>MakeSpanArray;
	MakeSpanArray.resize(TotaltestIns);
	for (int i = 0; i < MakeSpanArray.size(); i++)
		MakeSpanArray[i].resize(Reps, 0);

	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		string FileName = "DPFSPTestInstance\\TestFiles.txt", FileDirectory = "DPFSPTestInstance\\";
		ifstream ifile;
		ifile.open(FileName);
		string TestFileName;
		for (int xx = 0; xx <= ins; xx++)
			ifile >> TestFileName;
		ifile.close();
		TestFileName = FileDirectory + TestFileName;
		GetInstances_TaiE(gJobs, gMachine, gFactory, pTime, pSetupTime, TestFileName);

		long TimeLimit = 5 * gJobs * gMachine;      //
		for (int rep = 0; rep < Reps; rep++)
		{
			long start = GetElapsedProcessTime();

			srand(ins * 100 + rep);
			int Span = IGM(TimeLimit, Plen, Noimprovement, PlenWay);
			cout << "\t" << ins << "\t" << gJobs << "\t" << gMachine << "\t" << Span << "\t" << (GetElapsedProcessTime() - start) / 1000.0 << endl;
			MakeSpanArray[ins][rep] = Span;
		}
	}
//	string FileDirectory = "DPFSPTestInstance\\IGMTestResult\\";         //
	ostringstream str;
	str << "IGM_" << No << ".txt";                //
	ofstream ofile;
	ofile.open(str.str());
	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		for (int rep = 0; rep < Reps; rep++)
		{
			ofile << setw(8) << MakeSpanArray[ins][rep];
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}

void IGM_Calibration()
{
	srand(1994);

	//Read IP addresses
	vector<string> IPArray;
	ifstream ifile;
	string IPFileName = "IP.txt";
	ifile.open(IPFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			string str;
			ifile >> str;
			if (str != "")
				IPArray.push_back(str);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << IPFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	//Read configurations
	vector<int> RunSeq;
	string ConfFileName = "Confs.txt";
	ifile.open(ConfFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			int Conf;
			ifile >> Conf;
			if (Conf == -1) break;
			RunSeq.push_back(Conf);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << ConfFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	random_shuffle(RunSeq.begin(), RunSeq.end());
	for (int i = 0; i < RunSeq.size(); i++)
		cout << RunSeq[i] << endl;
	string LocalIP;
	FindIP(LocalIP);
	for (int i = 0; i < RunSeq.size(); i++)
	{
		string destIP = IPArray[i%IPArray.size()];
		if (LocalIP == destIP)
		{
			cout << i << "\t" << RunSeq[i] << "\t" << LocalIP << "\t" << endl;
			IGM_Test(RunSeq[i]);
		}
	}
}

//Get finaltext
void GetFinaltext_IGR()               //
{
	const int TotalIns = 20, Reps = 10, Confs = 4 * 5 * 4;       //
	int SpanArray[TotalIns][Confs*Reps];
	string Dir = "DPFSPTestInstance\\IGMTestResult\\";         //
	for (int i = 0; i < Confs; i++)
	{
		ostringstream outstr;
		outstr << i;
		string FName = Dir + "IGM_";                          //
		FName = FName + outstr.str() + ".txt";
		ifstream ifile;
		ifile.open(FName);
		if (!ifile)
		{
			cout << "File is not existed!" << endl;
			exit(0);
		}
		for (int Ins = 0; Ins < TotalIns; Ins++)
		{
			for (int Rep = 0; Rep < Reps; Rep++)
			{
				ifile >> SpanArray[Ins][i*Reps + Rep];
			}
		}
		ifile.close();
	}
	int minSpanArray[TotalIns];
	for (int Ins = 0; Ins < TotalIns; Ins++)
	{
		minSpanArray[Ins] = INT_MAX;
		for (int i = 0; i < Confs*Reps; i++)
		{
			if (SpanArray[Ins][i] < minSpanArray[Ins])
			{
				minSpanArray[Ins] = SpanArray[Ins][i];
			}
		}
	}

	int PlenArray[] = { 1, 3, 5, 7 };          //   
	int NoimprovementArray[] = { 1, 3, 5, 7, 9 };       //
	int PlenWayArray[] = { 0, 1, 2, 3 };

	string FileDirectory = "DPFSPTestInstance\\IGMTestResult\\";          //
	ostringstream str;
	str << "Cal_IGM" << ".txt";               //
	ofstream ofile;
	ofile.open(FileDirectory + str.str());

	ofile << "Ins\t";
	ofile << "Plen\t";            //
	ofile << "Noimprovement\t";             //
	ofile << "PlenWay\t";             //
	ofile << "Span\t";
	ofile << "RPI \t" << endl;

	for (int Ins = 0; Ins < TotalIns; Ins++)
	{
		for (int No = 0; No < Confs * Reps; No++)
		{
			//algorithmic parameters
			int Conf = No / Reps;
			int Plen = PlenArray[Conf / (5 * 4)];      //
			Conf = Conf % (5 * 4);                     //
			int Noimprovement = NoimprovementArray[Conf / 4];        //
			Conf = Conf % 4;
			int PlenWay = PlenWayArray[Conf];

			ofile << Ins << "\t";
			ofile << Plen << "\t";        //
			ofile << Noimprovement << "\t";         //
			ofile << PlenWay << "\t";         //
			ofile << SpanArray[Ins][No] << "\t";
			double RPD = (SpanArray[Ins][No] - minSpanArray[Ins])*100.0 / minSpanArray[Ins];
			ofile << RPD << endl;
		}
	}
	ofile.close();
}


//EA_Calibration
int EA_Test(int No)               //
{
	int NumIndiArray[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30, 40, 50 };         //   

	int Temp = No;
	int NumIndi = NumIndiArray[Temp];

	const int TotaltestIns = 20, Reps = 10;
	vector<vector<int>>MakeSpanArray;
	MakeSpanArray.resize(TotaltestIns);
	for (int i = 0; i < MakeSpanArray.size(); i++)
		MakeSpanArray[i].resize(Reps, 0);

	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		string FileName = "DPFSPTestInstance\\TestFiles.txt", FileDirectory = "DPFSPTestInstance\\";
		ifstream ifile;
		ifile.open(FileName);
		string TestFileName;
		for (int xx = 0; xx <= ins; xx++)
			ifile >> TestFileName;
		ifile.close();
		TestFileName = FileDirectory + TestFileName;
		GetInstances_TaiE(gJobs, gMachine, gFactory, pTime, pSetupTime, TestFileName);

		long TimeLimit = 5 * gJobs * gMachine;      //
		for (int rep = 0; rep < Reps; rep++)
		{
			long start = GetElapsedProcessTime();

			srand(ins * 100 + rep);
			int Span = EA(TimeLimit, NumIndi);
			cout << "\t" << ins << "\t" << gJobs << "\t" << gMachine << "\t" << Span << "\t" << (GetElapsedProcessTime() - start) / 1000.0 << endl;
			MakeSpanArray[ins][rep] = Span;
		}
	}
	string FileDirectory = "DPFSPTestInstance\\EATestResult\\";         //
	ostringstream str;
	str << "EA_" << No << ".txt";                //
	ofstream ofile;
	ofile.open(FileDirectory + str.str());
	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		for (int rep = 0; rep < Reps; rep++)
		{
			ofile << setw(8) << MakeSpanArray[ins][rep];
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}

void EA_Calibration()
{
	srand(1994);

	//Read IP addresses
	vector<string> IPArray;
	ifstream ifile;
	string IPFileName = "IP.txt";
	ifile.open(IPFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			string str;
			ifile >> str;
			if (str != "")
				IPArray.push_back(str);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << IPFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	//Read configurations
	vector<int> RunSeq;
	string ConfFileName = "Confs.txt";
	ifile.open(ConfFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			int Conf;
			ifile >> Conf;
			if (Conf == -1) break;
			RunSeq.push_back(Conf);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << ConfFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	random_shuffle(RunSeq.begin(), RunSeq.end());
	for (int i = 0; i < RunSeq.size(); i++)
		cout << RunSeq[i] << endl;
	string LocalIP;
	FindIP(LocalIP);
	for (int i = 0; i < RunSeq.size(); i++)
	{
		string destIP = IPArray[i%IPArray.size()];
		if (LocalIP == destIP)
		{
			cout << i << "\t" << RunSeq[i] << "\t" << LocalIP << "\t" << endl;
			EA_Test(RunSeq[i]);
		}
	}
}

//IG
//IGM_Calibration
int IG_Compared_Test(int No)               //
{
	int PlenArray[] = { 1, 2, 3, 4, 5, 6, 7, 8 };          //   
	double TemArray[] = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 1.0, 1.5 };      //

	int Temp = No;
	int Plen = PlenArray[Temp / 8];       //
	Temp = Temp % 8;                         //
	double Tem = TemArray[Temp];

	const int TotaltestIns = 20, Reps = 10;
	vector<vector<int>>MakeSpanArray;
	MakeSpanArray.resize(TotaltestIns);
	for (int i = 0; i < MakeSpanArray.size(); i++)
		MakeSpanArray[i].resize(Reps, 0);

	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		string FileName = "DPFSPTestInstance\\TestFiles.txt", FileDirectory = "DPFSPTestInstance\\";
		ifstream ifile;
		ifile.open(FileName);
		string TestFileName;
		for (int xx = 0; xx <= ins; xx++)
			ifile >> TestFileName;
		ifile.close();
		TestFileName = FileDirectory + TestFileName;
		GetInstances_TaiE(gJobs, gMachine, gFactory, pTime, pSetupTime, TestFileName);

		long TimeLimit = 5 * gJobs * gMachine;      //
		for (int rep = 0; rep < Reps; rep++)
		{
			long start = GetElapsedProcessTime();

			srand(ins * 100 + rep);
			int Span = IG_Compared(TimeLimit, Plen, Tem);
			cout << "\t" << ins << "\t" << gJobs << "\t" << gMachine << "\t" << Span << "\t" << (GetElapsedProcessTime() - start) / 1000.0 << endl;
			MakeSpanArray[ins][rep] = Span;
		}
	}
	string FileDirectory = "DPFSPTestInstance\\IGComparedTestResult\\";         //
	ostringstream str;
	str << "IGCompared_" << No << ".txt";                //
	ofstream ofile;
	ofile.open(FileDirectory + str.str());
	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		for (int rep = 0; rep < Reps; rep++)
		{
			ofile << setw(8) << MakeSpanArray[ins][rep];
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}

void IG_Compared_Calibration()
{
	srand(1994);

	//Read IP addresses
	vector<string> IPArray;
	ifstream ifile;
	string IPFileName = "IP.txt";
	ifile.open(IPFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			string str;
			ifile >> str;
			if (str != "")
				IPArray.push_back(str);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << IPFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	//Read configurations
	vector<int> RunSeq;
	string ConfFileName = "Confs.txt";
	ifile.open(ConfFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			int Conf;
			ifile >> Conf;
			if (Conf == -1) break;
			RunSeq.push_back(Conf);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << ConfFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	random_shuffle(RunSeq.begin(), RunSeq.end());
	for (int i = 0; i < RunSeq.size(); i++)
		cout << RunSeq[i] << endl;
	string LocalIP;
	FindIP(LocalIP);
	for (int i = 0; i < RunSeq.size(); i++)
	{
		string destIP = IPArray[i%IPArray.size()];
		if (LocalIP == destIP)
		{
			cout << i << "\t" << RunSeq[i] << "\t" << LocalIP << "\t" << endl;
			IG_Compared_Test(RunSeq[i]);
		}
	}
}

//CRO_Calibration
int CRO_Test(int No)                         //
{
	int InitialKEArray[] = { 50, 150, 250, 350 };
	double KELossRateArray[] = { 0.0, 0.5, 0.7, 1.0 };              //    
	int DethArray[] = { 1, 10, 20 };       //
	int SythArray[] = { 20, 50, 100 };       //

	int Temp = No;
	int InitialKE = InitialKEArray[Temp / (4 * 3 * 3)];        //
	Temp = Temp % (4 * 3 * 3);
	double KELossRate = KELossRateArray[Temp / (3 * 3)];               //
	Temp = Temp % (3 * 3);
	int Deth = DethArray[Temp / 3];                   //
	Temp = Temp % 3;
	int Syth = SythArray[Temp];

	const int TotaltestIns = 20, Reps = 10;
	vector<vector<int>>MakeSpanArray;
	MakeSpanArray.resize(TotaltestIns);
	for (int i = 0; i < MakeSpanArray.size(); i++)
		MakeSpanArray[i].resize(Reps, 0);

	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		string FileName = "DPFSPTestInstance\\TestFiles.txt", FileDirectory = "DPFSPTestInstance\\";
		ifstream ifile;
		ifile.open(FileName);
		string TestFileName;
		for (int xx = 0; xx <= ins; xx++)
			ifile >> TestFileName;
		ifile.close();
		TestFileName = FileDirectory + TestFileName;
		GetInstances_TaiE(gJobs, gMachine, gFactory, pTime, pSetupTime, TestFileName);

		long TimeLimit = 5 * gJobs * gMachine;      //
		for (int rep = 0; rep < Reps; rep++)
		{
			long start = GetElapsedProcessTime();
			srand(ins * 100 + rep);              //
			int Span = CRO(TimeLimit, InitialKE, KELossRate, Deth, Syth);

			if (Span == 0)
				cout << "########@@@@@@@%%%%%%%&&&&&&&&&&&&" << endl;

			cout << "\t" << ins << "\t" << gJobs << "\t" << gMachine << "\t" << Span << "\t" << (GetElapsedProcessTime() - start) / 1000.0 << endl;
			MakeSpanArray[ins][rep] = Span;
		}
	}
	string FileDirectory = "DPFSPTestInstance\\CROTestResult\\";         //
	ostringstream str;
	str << "CRO_" << No << ".txt";                //
	ofstream ofile;
	ofile.open(FileDirectory + str.str());
	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		for (int rep = 0; rep < Reps; rep++)
		{
			ofile << setw(8) << MakeSpanArray[ins][rep];
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}

void CRO_Calibration()
{
	srand(1994);

	//Read IP addresses
	vector<string> IPArray;
	ifstream ifile;
	string IPFileName = "IP.txt";
	ifile.open(IPFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			string str;
			ifile >> str;
			if (str != "")
				IPArray.push_back(str);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << IPFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	//Read configurations
	vector<int> RunSeq;
	string ConfFileName = "Confs.txt";
	ifile.open(ConfFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			int Conf;
			ifile >> Conf;
			if (Conf == -1) break;
			RunSeq.push_back(Conf);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << ConfFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	random_shuffle(RunSeq.begin(), RunSeq.end());
	for (int i = 0; i < RunSeq.size(); i++)
		cout << RunSeq[i] << endl;
	string LocalIP;
	FindIP(LocalIP);
	for (int i = 0; i < RunSeq.size(); i++)
	{
		string destIP = IPArray[i%IPArray.size()];
		if (LocalIP == destIP)
		{
			cout << i << "\t" << RunSeq[i] << "\t" << LocalIP << "\t" << endl;
			CRO_Test(RunSeq[i]);
		}
	}
}

//SS_Calibration
int SS_Test(int No)                         //
{
	int HsizeArray[] = { 2, 5, 10, 15 };           //    
	int SsizeArray[] = { 2, 5, 10, 15 };           // 
	int PageArray[] = { 1, 5, 10, 20 };     //

	int Temp = No;
	int Hsize = HsizeArray[Temp / (4 * 4)];        //
	Temp = Temp % (4 * 4);                         //
	int Ssize = SsizeArray[Temp / 4];
	Temp = Temp % 4;
	int Page = PageArray[Temp];                //

	const int TotaltestIns = 20, Reps = 10;
	vector<vector<int>>MakeSpanArray;
	MakeSpanArray.resize(TotaltestIns);
	for (int i = 0; i < MakeSpanArray.size(); i++)
		MakeSpanArray[i].resize(Reps, 0);

	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		string FileName = "DPFSPTestInstance\\TestFiles.txt", FileDirectory = "DPFSPTestInstance\\";
		ifstream ifile;
		ifile.open(FileName);
		string TestFileName;
		for (int xx = 0; xx <= ins; xx++)
			ifile >> TestFileName;
		ifile.close();
		TestFileName = FileDirectory + TestFileName;
		GetInstances_TaiE(gJobs, gMachine, gFactory, pTime, pSetupTime, TestFileName);

		long TimeLimit = 5 * gJobs * gMachine;      //
		for (int rep = 0; rep < Reps; rep++)
		{
			long start = GetElapsedProcessTime();

			srand(ins * 100 + rep);              //
			int Span = SS(TimeLimit, Hsize, Ssize, Page);   //

			cout << "\t" << ins << "\t" << gJobs << "\t" << gMachine << "\t" << Span << "\t" << (GetElapsedProcessTime() - start) / 1000.0 << endl;
			MakeSpanArray[ins][rep] = Span;
		}
	}
//	string FileDirectory = "DPFSPTestInstance\\SSTestResult\\";         //
	ostringstream str;
	str << "SS_" << No << ".txt";                //
	ofstream ofile;
	ofile.open(str.str());
	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		for (int rep = 0; rep < Reps; rep++)
		{
			ofile << setw(8) << MakeSpanArray[ins][rep];
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}

void SS_Calibration()
{
	srand(1994);

	//Read IP addresses
	vector<string> IPArray;
	ifstream ifile;
	string IPFileName = "IP.txt";
	ifile.open(IPFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			string str;
			ifile >> str;
			if (str != "")
				IPArray.push_back(str);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << IPFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	//Read configurations
	vector<int> RunSeq;
	string ConfFileName = "Confs.txt";
	ifile.open(ConfFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			int Conf;
			ifile >> Conf;
			if (Conf == -1) break;
			RunSeq.push_back(Conf);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << ConfFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	random_shuffle(RunSeq.begin(), RunSeq.end());
	for (int i = 0; i < RunSeq.size(); i++)
		cout << RunSeq[i] << endl;
	string LocalIP;
	FindIP(LocalIP);
	for (int i = 0; i < RunSeq.size(); i++)
	{
		string destIP = IPArray[i%IPArray.size()];
		if (LocalIP == destIP)
		{
			cout << i << "\t" << RunSeq[i] << "\t" << LocalIP << "\t" << endl;
			SS_Test(RunSeq[i]);
		}
	}
}


int IG_TwoSwap_Test(int No)               //
{
	int PlenArray[] = { 1, 3, 5, 7 };          //   
	int NoimprovementArray[] = { 3, 5, 7, 10, 15 };       //
	int NeighborArray[] = { 10, 20, 30, 40, 50, 60 };

	int Temp = No;
	int Plen = PlenArray[Temp / (5 * 6)];       //
	Temp = Temp % (5 * 6);                         //
	int Noimprovement = NoimprovementArray[Temp / 6];                //
	Temp = Temp % 6;
	int Neighbor = NeighborArray[Temp];

	const int TotaltestIns = 20, Reps = 10;
	vector<vector<int>>MakeSpanArray;
	MakeSpanArray.resize(TotaltestIns);
	for (int i = 0; i < MakeSpanArray.size(); i++)
		MakeSpanArray[i].resize(Reps, 0);

	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		string FileName = "DPFSPTestInstance\\TestFiles.txt", FileDirectory = "DPFSPTestInstance\\";
		ifstream ifile;
		ifile.open(FileName);
		string TestFileName;
		for (int xx = 0; xx <= ins; xx++)
			ifile >> TestFileName;
		ifile.close();
		TestFileName = FileDirectory + TestFileName;
		GetInstances_TaiE(gJobs, gMachine, gFactory, pTime, pSetupTime, TestFileName);

		long TimeLimit = 5 * gJobs * gMachine;      //
		for (int rep = 0; rep < Reps; rep++)
		{
			long start = GetElapsedProcessTime();

			srand(ins * 100 + rep);
			int Span = IG_TwoSwap(TimeLimit, Plen, Noimprovement, Neighbor);
			cout << "\t" << ins << "\t" << gJobs << "\t" << gMachine << "\t" << Span << "\t" << (GetElapsedProcessTime() - start) / 1000.0 << endl;
			MakeSpanArray[ins][rep] = Span;
		}
	}
	//	string FileDirectory = "DPFSPTestInstance\\IGMTestResult\\";         //
	ostringstream str;
	str << "IG_TwoSwap_" << No << ".txt";                //
	ofstream ofile;
	ofile.open(str.str());
	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		for (int rep = 0; rep < Reps; rep++)
		{
			ofile << setw(8) << MakeSpanArray[ins][rep];
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}

void IG_TwoSwap_Calibration(int Parts, int PartNo)       //
{
	srand(1994);
	vector <int> NoArray;
	for (int i = 0; i < 4 * 5 * 6; i++)      //
		NoArray.push_back(i);
	random_shuffle(NoArray.begin(), NoArray.end());

	int div = NoArray.size() / Parts;
	int Start = PartNo * div;
	int End = min(Start + div, NoArray.size());

	for (int i = Start; i < End; i++)
		IG_TwoSwap_Test(NoArray[i]);           //
}

int IG_ThreeSwap_Test(int No)               //
{
	int PlenArray[] = { 1, 3, 5, 7 };          //   
	int NoimprovementArray[] = { 3, 5, 7, 10, 15 };       //
	int NeighborArray[] = { 10, 20, 30, 40, 50, 60 };

	int Temp = No;
	int Plen = PlenArray[Temp / (5 * 6)];       //
	Temp = Temp % (5 * 6);                         //
	int Noimprovement = NoimprovementArray[Temp / 6];                //
	Temp = Temp % 6;
	int Neighbor = NeighborArray[Temp];

	const int TotaltestIns = 20, Reps = 10;
	vector<vector<int>>MakeSpanArray;
	MakeSpanArray.resize(TotaltestIns);
	for (int i = 0; i < MakeSpanArray.size(); i++)
		MakeSpanArray[i].resize(Reps, 0);

	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		string FileName = "DPFSPTestInstance\\TestFiles.txt", FileDirectory = "DPFSPTestInstance\\";
		ifstream ifile;
		ifile.open(FileName);
		string TestFileName;
		for (int xx = 0; xx <= ins; xx++)
			ifile >> TestFileName;
		ifile.close();
		TestFileName = FileDirectory + TestFileName;
		GetInstances_TaiE(gJobs, gMachine, gFactory, pTime, pSetupTime, TestFileName);

		long TimeLimit = 5 * gJobs * gMachine;      //
		for (int rep = 0; rep < Reps; rep++)
		{
			long start = GetElapsedProcessTime();

			srand(ins * 100 + rep);
			int Span = IG_ThreeSwap(TimeLimit, Plen, Noimprovement, Neighbor);
			cout << "\t" << ins << "\t" << gJobs << "\t" << gMachine << "\t" << Span << "\t" << (GetElapsedProcessTime() - start) / 1000.0 << endl;
			MakeSpanArray[ins][rep] = Span;
		}
	}
	//	string FileDirectory = "DPFSPTestInstance\\IGMTestResult\\";         //
	ostringstream str;
	str << "IG_ThreeSwap_" << No << ".txt";                //
	ofstream ofile;
	ofile.open(str.str());
	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		for (int rep = 0; rep < Reps; rep++)
		{
			ofile << setw(8) << MakeSpanArray[ins][rep];
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}

void IG_ThreeSwap_Calibration(int Parts, int PartNo)       //
{
	srand(1994);
	vector <int> NoArray;
	for (int i = 0; i < 4 * 5 * 6; i++)      //
		NoArray.push_back(i);
	random_shuffle(NoArray.begin(), NoArray.end());

	int div = NoArray.size() / Parts;
	int Start = PartNo * div;
	int End = min(Start + div, NoArray.size());

	for (int i = Start; i < End; i++)
		IG_ThreeSwap_Test(NoArray[i]);           //
}

//DABC_Calibration
int DABC_Test(int No)               //
{
	int PsizeArray[] = { 2, 6, 10, 14, 18, 20 };          //   
	int GlimitArray[] = { 1, 3, 5, 7, 9, 15, 20, 25, 30, 40 };       //

	int Temp = No;
	int Psize = PsizeArray[Temp / 10];       //
	Temp = Temp % 10;                        //
	int Glimit = GlimitArray[Temp];                //

	const int TotaltestIns = 20, Reps = 10;
	vector<vector<int>>MakeSpanArray;
	MakeSpanArray.resize(TotaltestIns);
	for (int i = 0; i < MakeSpanArray.size(); i++)
		MakeSpanArray[i].resize(Reps, 0);

	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		string FileName = "DPFSPTestInstance\\TestFiles.txt", FileDirectory = "DPFSPTestInstance\\";
		ifstream ifile;
		ifile.open(FileName);
		string TestFileName;
		for (int xx = 0; xx <= ins; xx++)
			ifile >> TestFileName;
		ifile.close();
		TestFileName = FileDirectory + TestFileName;
		GetInstances_TaiE(gJobs, gMachine, gFactory, pTime, pSetupTime, TestFileName);

		long TimeLimit = 5 * gJobs * gMachine;      //
		for (int rep = 0; rep < Reps; rep++)
		{
			long start = GetElapsedProcessTime();

			srand(ins * 100 + rep);
			int Span = DABC_Compared(TimeLimit, Psize, Glimit);
			cout << "\t" << ins << "\t" << gJobs << "\t" << gMachine << "\t" << Span << "\t" << (GetElapsedProcessTime() - start) / 1000.0 << endl;
			MakeSpanArray[ins][rep] = Span;
		}
	}
	//	string FileDirectory = "DPFSPTestInstance\\IGMTestResult\\";         //
	ostringstream str;
	str << "DABC_" << No << ".txt";                //
	ofstream ofile;
	ofile.open(str.str());
	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		for (int rep = 0; rep < Reps; rep++)
		{
			ofile << setw(8) << MakeSpanArray[ins][rep];
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}

void DABC_Calibration()
{
	srand(1994);

	//Read IP addresses
	vector<string> IPArray;
	ifstream ifile;
	string IPFileName = "IP.txt";
	ifile.open(IPFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			string str;
			ifile >> str;
			if (str != "")
				IPArray.push_back(str);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << IPFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	//Read configurations
	vector<int> RunSeq;
	string ConfFileName = "Confs.txt";
	ifile.open(ConfFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			int Conf;
			ifile >> Conf;
			if (Conf == -1) break;
			RunSeq.push_back(Conf);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << ConfFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	random_shuffle(RunSeq.begin(), RunSeq.end());
	for (int i = 0; i < RunSeq.size(); i++)
		cout << RunSeq[i] << endl;
	string LocalIP;
	FindIP(LocalIP);
	for (int i = 0; i < RunSeq.size(); i++)
	{
		string destIP = IPArray[i%IPArray.size()];
		if (LocalIP == destIP)
		{
			cout << i << "\t" << RunSeq[i] << "\t" << LocalIP << "\t" << endl;
			DABC_Test(RunSeq[i]);
		}
	}
}

//Get finaltext
void GetFinaltext_DABC()               //
{
	const int TotalIns = 20, Reps = 10, Confs = 6 * 10;       //
	int SpanArray[TotalIns][Confs*Reps];
	string Dir = "DPFSPTestInstance\\DABCTestResult\\";         //
	for (int i = 0; i < Confs; i++)
	{
		ostringstream outstr;
		outstr << i;
		string FName = Dir + "DABC_";                          //
		FName = FName + outstr.str() + ".txt";
		ifstream ifile;
		ifile.open(FName);
		if (!ifile)
		{
			cout << "File is not existed!" << endl;
			exit(0);
		}
		for (int Ins = 0; Ins < TotalIns; Ins++)
		{
			for (int Rep = 0; Rep < Reps; Rep++)
			{
				ifile >> SpanArray[Ins][i*Reps + Rep];
			}
		}
		ifile.close();
	}
	int minSpanArray[TotalIns];
	for (int Ins = 0; Ins < TotalIns; Ins++)
	{
		minSpanArray[Ins] = INT_MAX;
		for (int i = 0; i < Confs*Reps; i++)
		{
			if (SpanArray[Ins][i] < minSpanArray[Ins])
			{
				minSpanArray[Ins] = SpanArray[Ins][i];
			}
		}
	}

	int PsizeArray[] = { 2, 6, 10, 14, 18, 20 };          //   
	int GlimitArray[] = { 1, 3, 5, 7, 9, 15, 20, 25, 30, 40 };       //

	string FileDirectory = "DPFSPTestInstance\\DABCTestResult\\";          //
	ostringstream str;
	str << "Cal_DABC" << ".txt";               //
	ofstream ofile;
	ofile.open(FileDirectory + str.str());

	ofile << "Ins\t";
	ofile << "Psize\t";            //
	ofile << "Glimit\t";             //
	ofile << "Span\t";
	ofile << "RPI \t" << endl;

	for (int Ins = 0; Ins < TotalIns; Ins++)
	{
		for (int No = 0; No < Confs * Reps; No++)
		{
			//algorithmic parameters
			int Conf = No / Reps;
			int Psize = PsizeArray[Conf / 10];      //
			Conf = Conf % 10;                     //
			int Glimit = GlimitArray[Conf];        //

			ofile << Ins << "\t";
			ofile << Psize << "\t";        //
			ofile << Glimit << "\t";         //
			ofile << SpanArray[Ins][No] << "\t";
			double RPD = (SpanArray[Ins][No] - minSpanArray[Ins])*100.0 / minSpanArray[Ins];
			ofile << RPD << endl;
		}
	}
	ofile.close();
}


//GA_Calibration
int GA_Test(int No)               //
{
	int PsizeArray[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 15, 30, 35, 40 };          //   

	int Temp = No;
	int Psize = PsizeArray[Temp];       //

	const int TotaltestIns = 20, Reps = 10;
	vector<vector<int>>MakeSpanArray;
	MakeSpanArray.resize(TotaltestIns);
	for (int i = 0; i < MakeSpanArray.size(); i++)
		MakeSpanArray[i].resize(Reps, 0);

	for (int ins = 10; ins < TotaltestIns; ins++)
	{
		string FileName = "DPFSPTestInstance\\TestFiles.txt", FileDirectory = "DPFSPTestInstance\\";
		ifstream ifile;
		ifile.open(FileName);
		string TestFileName;
		for (int xx = 0; xx <= ins; xx++)
			ifile >> TestFileName;
		ifile.close();
		TestFileName = FileDirectory + TestFileName;
		GetInstances_TaiE(gJobs, gMachine, gFactory, pTime, pSetupTime, TestFileName);

		long TimeLimit = 5 * gJobs * gMachine;      //
		for (int rep = 0; rep < Reps; rep++)
		{
			long start = GetElapsedProcessTime();

			srand(ins * 100 + rep);
			int Span = GA(TimeLimit, Psize);
			cout << "\t" << ins << "\t" << gJobs << "\t" << gMachine << "\t" << Span << "\t" << (GetElapsedProcessTime() - start) / 1000.0 << endl;
			MakeSpanArray[ins][rep] = Span;
		}
	}
	//	string FileDirectory = "DPFSPTestInstance\\IGMTestResult\\";         //
	ostringstream str;
	str << "GA_" << No << ".txt";                //
	ofstream ofile;
	ofile.open(str.str());
	for (int ins = 0; ins < TotaltestIns; ins++)
	{
		for (int rep = 0; rep < Reps; rep++)
		{
			ofile << setw(8) << MakeSpanArray[ins][rep];
		}
		ofile << endl;
	}
	ofile.close();
	return 0;
}

void GA_Calibration()
{
	srand(1994);

	//Read IP addresses
	vector<string> IPArray;
	ifstream ifile;
	string IPFileName = "IP.txt";
	ifile.open(IPFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			string str;
			ifile >> str;
			if (str != "")
				IPArray.push_back(str);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << IPFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	//Read configurations
	vector<int> RunSeq;
	string ConfFileName = "Confs.txt";
	ifile.open(ConfFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			int Conf;
			ifile >> Conf;
			if (Conf == -1) break;
			RunSeq.push_back(Conf);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << ConfFileName << "  is not existed." << endl;
		getchar();
		exit(0);
	}

	random_shuffle(RunSeq.begin(), RunSeq.end());
	for (int i = 0; i < RunSeq.size(); i++)
		cout << RunSeq[i] << endl;
	string LocalIP;
	FindIP(LocalIP);
	for (int i = 0; i < RunSeq.size(); i++)
	{
		string destIP = IPArray[i%IPArray.size()];
		if (LocalIP == destIP)
		{
			cout << i << "\t" << RunSeq[i] << "\t" << LocalIP << "\t" << endl;
			GA_Test(RunSeq[i]);
		}
	}
}

//Get finaltext
void GetFinaltext_GA()               //
{
	const int TotalIns = 20, Reps = 10, Confs = 15;       //
	int SpanArray[TotalIns][Confs*Reps];
	string Dir = "DPFSPTestInstance\\GATestResult\\";         //
	for (int i = 0; i < Confs; i++)
	{
		ostringstream outstr;
		outstr << i;
		string FName = Dir + "GA_";                          //
		FName = FName + outstr.str() + ".txt";
		ifstream ifile;
		ifile.open(FName);
		if (!ifile)
		{
			cout << "File is not existed!" << endl;
			exit(0);
		}
		for (int Ins = 0; Ins < TotalIns; Ins++)
		{
			for (int Rep = 0; Rep < Reps; Rep++)
			{
				ifile >> SpanArray[Ins][i*Reps + Rep];
			}
		}
		ifile.close();
	}
	int minSpanArray[TotalIns];
	for (int Ins = 0; Ins < TotalIns; Ins++)
	{
		minSpanArray[Ins] = INT_MAX;
		for (int i = 0; i < Confs*Reps; i++)
		{
			if (SpanArray[Ins][i] < minSpanArray[Ins])
			{
				minSpanArray[Ins] = SpanArray[Ins][i];
			}
		}
	}

	int PsizeArray[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 15, 30, 35, 40 };

	string FileDirectory = "DPFSPTestInstance\\GATestResult\\";          //
	ostringstream str;
	str << "Cal_GA" << ".txt";               //
	ofstream ofile;
	ofile.open(FileDirectory + str.str());

	ofile << "Ins\t";
	ofile << "Psize\t";            //
	ofile << "Span\t";
	ofile << "RPI \t" << endl;

	for (int Ins = 0; Ins < TotalIns; Ins++)
	{
		for (int No = 0; No < Confs * Reps; No++)
		{
			//algorithmic parameters
			int Conf = No / Reps;
			int Psize = PsizeArray[Conf];      //

			ofile << Ins << "\t";
			ofile << Psize << "\t";        //
			ofile << SpanArray[Ins][No] << "\t";
			double RPD = (SpanArray[Ins][No] - minSpanArray[Ins])*100.0 / minSpanArray[Ins];
			ofile << RPD << endl;
		}
	}
	ofile.close();
}