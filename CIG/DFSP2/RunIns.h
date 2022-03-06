#pragma once
//---------InsOutput.h----

void OutputPerIns()
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

	//Read Ins
	vector<int> RunSeq;
	string InsFileName = "Ins.txt";
	ifile.open(InsFileName);
	if (ifile.is_open())
	{
		while (true)
		{
			int ins;
			ifile >> ins;
			if (ins == -1) break;
			RunSeq.push_back(ins);
			if (ifile.peek() == EOF)
				break;
		}
		ifile.close();
	}
	else
	{
		cout << InsFileName << "  is not existed." << endl;
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
			for (int CPU = 10; CPU <= 30; CPU = CPU + 10)
			{
				EA_Run(CPU, RunSeq[i]);
				IGCompared_Run(CPU, RunSeq[i]);
				SS_Run(CPU, RunSeq[i]);
				CRO_Run(CPU, RunSeq[i]);
				IGM_Run(CPU, RunSeq[i]);
			}				
		}
	}
}

void GetFinalResult()
{
	for (int CPU = 1; CPU <= 3; CPU++)
	{
		string OpenFileDirectory = "DPFSPTestInstance\\FianlResult\\";
		for (int Alg = 0; Alg < 6; Alg++)
		{
			switch (Alg)
			{
			case 0:
			{
					  ostringstream str;
					  str << "IGM_" << CPU * 10 << ".txt";              //
					  ofstream ofile;
					  ofile.open(OpenFileDirectory + str.str());

					  for (int Ins = 0; Ins < 270; Ins++)
					  {
						  vector<int>MakeSpan;
						  MakeSpan.resize(5, 0);
						  string Dir = "DPFSPTestInstance\\FianlAllResult\\";
						  ostringstream str;
						  str << "IGM_" << CPU * 10 << "_" << Ins << ".txt";
						  ifstream ifile;
						  ifile.open(Dir + str.str());               //
						  if (!ifile)
						  {
							  cout << "File is not existed!" << endl;
							  exit(0);
						  }
						  else
						  {
							  for (int Rep = 0; Rep < 5; Rep++)
								  ifile >> MakeSpan[Rep];
						  }
						  ifile.close();

						  for (int Rep = 0; Rep < 5;Rep++)
							  ofile << MakeSpan[Rep] << "\t";
						  ofile << endl;
					  }
					  ofile.close();
					  break;
			}
			case 1:
			{
					  ostringstream str;
					  str << "IGCompared_" << CPU * 10 << ".txt";              //
					  ofstream ofile;
					  ofile.open(OpenFileDirectory + str.str());

					  for (int Ins = 0; Ins < 270; Ins++)
					  {
						  vector<int>MakeSpan;
						  MakeSpan.resize(5, 0);
						  string Dir = "DPFSPTestInstance\\FianlAllResult\\";
						  ostringstream str;
						  str << "IGCompared_" << CPU * 10 << "_" << Ins << ".txt";
						  ifstream ifile;
						  ifile.open(Dir + str.str());               //
						  if (!ifile)
						  {
							  cout << "File is not existed!" << endl;
							  exit(0);
						  }
						  else
						  {
							  for (int Rep = 0; Rep < 5; Rep++)
								  ifile >> MakeSpan[Rep];
						  }
						  ifile.close();

						  for (int Rep = 0; Rep < 5; Rep++)
							  ofile << MakeSpan[Rep] << "\t";
						  ofile << endl;
					  }
					  ofile.close();
					  break;
			}
			case 2:
			{
					  ostringstream str;
					  str << "CRO_" << CPU * 10 << ".txt";              //
					  ofstream ofile;
					  ofile.open(OpenFileDirectory + str.str());

					  for (int Ins = 0; Ins < 270; Ins++)
					  {
						  vector<int>MakeSpan;
						  MakeSpan.resize(5, 0);
						  string Dir = "DPFSPTestInstance\\FianlAllResult\\";
						  ostringstream str;
						  str << "CRO_" << CPU * 10 << "_" << Ins << ".txt";
						  ifstream ifile;
						  ifile.open(Dir + str.str());               //
						  if (!ifile)
						  {
							  cout << "File is not existed!" << endl;
							  exit(0);
						  }
						  else
						  {
							  for (int Rep = 0; Rep < 5; Rep++)
								  ifile >> MakeSpan[Rep];
						  }
						  ifile.close();

						  for (int Rep = 0; Rep < 5; Rep++)
							  ofile << MakeSpan[Rep] << "\t";
						  ofile << endl;
					  }
					  ofile.close();
					  break;
			}
			case 3:
			{
					  ostringstream str;
					  str << "DABC_" << CPU * 10 << ".txt";              //
					  ofstream ofile;
					  ofile.open(OpenFileDirectory + str.str());

					  for (int Ins = 0; Ins < 270; Ins++)
					  {
						  vector<int>MakeSpan;
						  MakeSpan.resize(5, 0);
						  string Dir = "DPFSPTestInstance\\FianlAllResult\\";
						  ostringstream str;
						  str << "DABC_" << CPU * 10 << "_" << Ins << ".txt";
						  ifstream ifile;
						  ifile.open(Dir + str.str());               //
						  if (!ifile)
						  {
							  cout << "File is not existed!" << endl;
							  exit(0);
						  }
						  else
						  {
							  for (int Rep = 0; Rep < 5; Rep++)
								  ifile >> MakeSpan[Rep];
						  }
						  ifile.close();

						  for (int Rep = 0; Rep < 5; Rep++)
							  ofile << MakeSpan[Rep] << "\t";
						  ofile << endl;
					  }
					  ofile.close();
					  break;
			}
			case 4:
			{
					  ostringstream str;
					  str << "EA_" << CPU * 10 << ".txt";              //
					  ofstream ofile;
					  ofile.open(OpenFileDirectory + str.str());

					  for (int Ins = 0; Ins < 270; Ins++)
					  {
						  vector<int>MakeSpan;
						  MakeSpan.resize(5, 0);
						  string Dir = "DPFSPTestInstance\\FianlAllResult\\";
						  ostringstream str;
						  str << "EA_" << CPU * 10 << "_" << Ins << ".txt";
						  ifstream ifile;
						  ifile.open(Dir + str.str());               //
						  if (!ifile)
						  {
							  cout << "File is not existed!" << endl;
							  exit(0);
						  }
						  else
						  {
							  for (int Rep = 0; Rep < 5; Rep++)
								  ifile >> MakeSpan[Rep];
						  }
						  ifile.close();

						  for (int Rep = 0; Rep < 5; Rep++)
							  ofile << MakeSpan[Rep] << "\t";
						  ofile << endl;
					  }
					  ofile.close();
					  break;
			}
			case 5:
			{
					  ostringstream str;
					  str << "GA_" << CPU * 10 << ".txt";              //
					  ofstream ofile;
					  ofile.open(OpenFileDirectory + str.str());

					  for (int Ins = 0; Ins < 270; Ins++)
					  {
						  vector<int>MakeSpan;
						  MakeSpan.resize(5, 0);
						  string Dir = "DPFSPTestInstance\\FianlAllResult\\";
						  ostringstream str;
						  str << "GA_" << CPU * 10 << "_" << Ins << ".txt";
						  ifstream ifile;
						  ifile.open(Dir + str.str());               //
						  if (!ifile)
						  {
							  cout << "File is not existed!" << endl;
							  exit(0);
						  }
						  else
						  {
							  for (int Rep = 0; Rep < 5; Rep++)
								  ifile >> MakeSpan[Rep];
						  }
						  ifile.close();

						  for (int Rep = 0; Rep < 5; Rep++)
							  ofile << MakeSpan[Rep] << "\t";
						  ofile << endl;
					  }
					  ofile.close();
					  break;
			}
			}				
		}
	}
}