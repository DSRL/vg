// VegCipherBracker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <regex>
#include <algorithm>
#include <map>
#include <numeric>
#include <set>
using namespace std;

#define MAXKEY 12
vector<pair<uint8_t, double_t>> voteForLen(const vector<vector<pair<uint8_t, double_t>>>& votes)
{
	vector<pair<uint8_t, double_t>> pollResultsOrd;
	map<uint8_t, double_t> pollResultsMap;
	for_each(votes.begin(), votes.end(), [&](auto voter)
	{
		double_t sum = 0.0;
		for_each(voter.begin(), voter.end(), [&](auto vote)
		{
			sum += vote.second;
		});
		for_each(voter.begin(), voter.end(), [&](auto vote)
		{
			pollResultsMap[vote.first] += vote.second/(sum/100);
		});
	});

	for (auto it = pollResultsMap.begin(); it != pollResultsMap.end(); ++it) {
		pollResultsOrd.push_back(make_pair(it->first,it->second));
	}
	sort(pollResultsOrd.begin(), pollResultsOrd.end(),
		[](auto &a, auto &b) -> bool
	{
		return a.second > b.second;
	});
	return pollResultsOrd;
}

vector<pair<uint8_t, double_t>> TrigramsTest(string dataLow)
{
	vector<pair<uint8_t, double_t>> charChart;
	map<uint32_t, uint32_t> distances;

	for (size_t i = 0; i < dataLow.size(); i++)
	{
		auto keyStr = dataLow.substr(i, 3);
		auto pos = dataLow.find(keyStr, i);
		auto prevPos = pos;
		while (true)
		{
			pos = dataLow.find(keyStr, ++pos);
			if (pos != string::npos)
			{
				//cout << prevPos << keyStr << pos << '\n';
				//cout << pos-prevPos << keyStr <<  '\n';
				distances[pos - prevPos] = distances[pos - prevPos]++;
				prevPos = pos;
			}
			else
			{
				break;
			}
		}
	}

	map <uint32_t, set<uint32_t>> distDivs;
	map <uint32_t, uint32_t> distHits;

	for_each(distances.begin(), distances.end(), [&](auto p) {
		// Note that this loop runs till square root
		auto n = p.first;
		for (int i = 2; i < MAXKEY; i++)
		{
			if (p.first%i == 0)
			{
					distHits[i] = distHits[i]+p.second;
			}
		}
	});
	if (distHits.size() > 2)
	{
		vector <pair<uint32_t, uint32_t>> ordDistHits;

		for_each(distHits.begin(), distHits.end(),
			[&ordDistHits](auto a)
		{
			ordDistHits.push_back(make_pair(a.first, a.second));
		});

		sort(ordDistHits.begin(), ordDistHits.end(),
			[](auto &a, auto &b) -> bool
		{
			return a.second > b.second;
		});
		if (ordDistHits.size() >= 5)
		{
			for_each(ordDistHits.begin(), ordDistHits.begin() + 5, [&](auto tmp) {
				cout << tmp.first << " " << tmp.second << '\n';
				charChart.push_back(make_pair(tmp.first, tmp.second));
			});
		}
		else
		{
			for_each(ordDistHits.begin(), ordDistHits.end(), [&](auto tmp) {
				cout << tmp.first << " " << tmp.second << '\n';
				charChart.push_back(make_pair(tmp.first, tmp.second));
			});
		}

	}
	else
	{
		cout << "Shift failed" << '\n';
	}

	cout << '\n';
	return charChart;
}

vector<pair<uint8_t, double_t>>  AutoCorrelation(string dataLow)
{
	vector<pair<uint8_t, double_t>> charChart;
	auto dataRot = dataLow;
	auto pureSize = dataLow.size();
	vector <pair<uint32_t, double_t>> ordDistHits;
	for (auto keyLen = 0; keyLen <= 12; keyLen++)
	{
		int counter = 0;
		for (auto i = 0; i < dataLow.size(); i++)
		{
			if (dataLow[i] == dataRot[i])
				counter++;
		}
		rotate(dataRot.begin(), dataRot.begin() + 1, dataRot.end());
		//cout << counter << "\t" << keyLen << "\t" << counter /(double)pureSize << endl;
		ordDistHits.push_back(make_pair(keyLen, counter / (double)pureSize));
	}

	sort(ordDistHits.begin(), ordDistHits.end(),
		[](auto &a, auto &b) -> bool
	{
		return a.second > b.second;
	});
	if (ordDistHits.size() >= 5)
	{
		for_each(ordDistHits.begin() + 1, ordDistHits.begin() + 6, [&](auto tmp) {
			cout << tmp.first << " " << tmp.second << '\n';
			charChart.push_back(make_pair(tmp.first, tmp.second));
		});
	}
	else
	{
		for_each(ordDistHits.begin() + 1, ordDistHits.end(), [&](auto tmp) {
			cout << tmp.first << " " << tmp.second << '\n';
			charChart.push_back(make_pair(tmp.first, tmp.second));
		});
	}
	cout << '\n';
	return charChart;
}

vector<pair<uint8_t, double_t>> CiCountTest(string dataLow)
{
	vector<pair<uint8_t, double_t>> charChart;
	vector <pair<uint32_t, double_t>> ordDistHits;
	for (int keyLen = 1; keyLen < 12; keyLen++)
	{
		double ciSum = 0;

		string tmp;
		for (auto i = dataLow.begin();;)
		{
			//cout << *i;
			tmp += *i;
			if (i < dataLow.end() - keyLen)
				i += keyLen;
			else
				break;
		}
		//cout << endl;
		auto elementSize = tmp.size();
		for (char curChar = 'a'; curChar <= 'z'; curChar++)
		{
			auto n = count(tmp.begin(), tmp.end(), curChar);
			double cn = (n * (n - 1));
			double celementSize = (elementSize * (elementSize - 1));
			double ci = cn / celementSize;
			ciSum += ci;
			//cout << curChar << " : " << n << " : " << ci << endl;
		}
		//cout << "Size\t" << elementSize << "\t";
		//cout << keyLen << "\tciSum\t" << ciSum << endl;
		ordDistHits.push_back(make_pair(keyLen, ciSum));
	}

	sort(ordDistHits.begin(), ordDistHits.end(),
		[](auto &a, auto &b) -> bool
	{
		return a.second > b.second;
	});
	if (ordDistHits.size() >= 5)
	{
		for_each(ordDistHits.begin(), ordDistHits.begin() + 5, [&](auto tmp) {
			cout << tmp.first << " " << tmp.second << '\n';
			charChart.push_back(make_pair(tmp.first, tmp.second));
		});
	}
	else
	{
		for_each(ordDistHits.begin() + 1, ordDistHits.end(), [&](auto tmp) {
			cout << tmp.first << " " << tmp.second << '\n';
			charChart.push_back(make_pair(tmp.first, tmp.second));
		});
	}
	return charChart;
}

int main()
{
	///*SYSTEM*/string input = "\
		LFWKI MJCLP SISWK HJOGL KMVGU RAGKM KMXMA MJCVX WUYLG GIISW\
		ALXAE YCXMF KMKBQ BDCLA EFLFW KIMJC GUZUG SKECZ GBWYM OACFV\
		MQKYF WXTWM LAIDO YQBWF GKSDI ULQGV SYHJA VEFWB LAEFL FWKIM\
		JCFHS NNGGN WPWDA VMQFA AXWFZ CXBVE LKWML AVGKY EDEMJ XHUXD\
		AVYXL. ";
	//string input = "LIOMWGFEGGDVWGHHCQUCRHRWAGWIOWQLKGZETKKMEVLW\
		PCZVG'TH VTSGXQOVGCSVETQLTJSUMVWVEUVLXEWSLGFZMVVWLGYHCUSWX\
		QH - KVGSHEEVFLCFDGVSUMPHKIRZDMPHHBVWVWJWIXGFWLTSHGJOUEEH\
		H- VUCFVGOWICQLIJSUXGLW";
	///*BOY*/string input = "NWAIW EBBRF QFOCJ PUGDO JVBGW SPTWR Z";
	/*required*/string input = "klym qj e erwu wtrwv kicjtrxh klqn krr ev yiyl ks gvvypm zxhiejiz tpdjwum nisp zx. yn qj rrk ed cbvvdksh wtrwv rrt xwvw qfx flwmmgv eds ww xkv jkhkkmreebcbp eq zxuliksu zw urxvgwvh ji przh.klym jrwh tpqma frop thidzhhj wegm dipsih nggiv, nlywp zr irgj uzv rrk vukczvhu xe vm gvhjidn qe eqp mjyzrxri xojm(zxhiejiz kcsvw xudv rr jtuwqwmf dicvmi vhhyylmdiqkw), rob klhp qyapk fh lwuzcc, wlegu npvc gvjyhm klh dicvmiw qvityl wsu klu xmwexcx ynmiewfv_jlizxv tpqma kipgpqnm ks jvruliki wyi qjxissimqnm zrvkednqrxlfr qobfqdkmsutcc(deh ioky mqjxqhbzewzsd ca iitlmhyl ks ev zqfqu jri ebf qkiurxel bpthj).";
	///*BA*/string input = "Uhjs f-mbim aod bnz fjlfs urbntmjtuee wjti iu mby dootbio cpngieeotjam iofprnauipn uhbt js qrjvjlfgfd pr ptieswjsf eyenpu fson djsdlpsvrf uodfr bpqljcbbme max. Aoy eitsfmjnbtjoo, djsurjbvtjoo, os cppz og tiit cpmnuoidauipn pties tiao tp tie jneiwieubl pr fnuiuy oanee acowe js ttsidtmy qrphjbjtfd. Jf zov ase oou tie jnueodfd bderfstef, npr buuhprjzfd uo sedejvf fpr uhf iotfneee aedsetsfe, zov ase iesecy oouigifd uhbt zov mby oou ute, doqy, eitcmote uo bnzooe uhf iofprnauipn dootbioee io os autbciee tp tiit mfstahe. Jf zov rfcfiwee tiit cpmnuoidauipn jn frsos, pmebsf inmfdjauemy bdwite cy seqlz e-najl bne dflftf tiit mfstahe, jtt autbcimfnus bne aoy doqifs.";
	///*BAG*/string input = "UHOTESBIRBNJBNEGIRFSZSATTMOUTKEWOUHOUMGZCUOTGJNIPNLJDKOTOBLOOFUSMGUIUOTNBTOTPXJVOMEMFDUSOZIEXXIYFEDFMVUFXPMJJSIMOYVRKVNJFRGQPRJCGCLKMACBNEEIYTESJNGUIUODOTTXJBAUIUOOXDOVZOLUHOTCUNMAOIIBTOPNUUHKSTNBNZPTNFITEIBJDABLUSETUIZZNGNEJBBUWEOTSZSIIULEQRUIIHJTKEILZOABRKOOZUHKJNZFNJFDGEDXFSYFETPRGVTNPROAEJUOXFCKJVKGOXUHKJNZFNJFDGEDXFSYFEEPUGSENFRKCYTPTOGIKETNBTEPUSBYTPTATEIPPEEIYDLUTEZPATZOTFTNFITGOXNAZJOTDOTUAOOEJJNUSAZUAIIEJUOZIIYNEYTAMFILZOASEIFIBFDZIIYDOSNUTJCGUIUOITFRXPRVMEGTEONMKEIGUERZAJWIYFBESEVMYKNAOMATEDKMEZFTNJSSFSYBGKJTYBTZBCNNETUSGODGOYIPPOFS";
	auto origSize = input.size();
	input = regex_replace(input, regex("\\s+"), "");
	auto whitespaceSize = input.size();
	input = regex_replace(input, regex("\\d+"), "");
	auto numberSize = input.size();
	input = regex_replace(input, regex("\\W+"), "");
	auto pureSize = input.size();
	cout << input<< endl;
	cout << "origSize "<<origSize<<endl
		<< "whitespace "<<origSize - whitespaceSize<<endl
		<< "numbers "<<whitespaceSize - numberSize<<endl
		<< "restOfSumbols " << numberSize - pureSize << endl
		<< "pureSize " << pureSize << endl;
	cout << '\n';
	auto dataLow = input;
	transform(dataLow.begin(), dataLow.end(), dataLow.begin(), ::tolower);
	vector<vector<pair<uint8_t,double_t>>> voters;
	voters.push_back(AutoCorrelation(dataLow));
	voters.push_back(TrigramsTest(dataLow));
	voters.push_back(CiCountTest(dataLow));
	
	auto PollRes = voteForLen(voters);
	for each (auto var in PollRes)
	{
		cout<<"keyLen is: " << static_cast<int>(var.first) <<" " << var.second << endl;
	}
	uint8_t keyLen = PollRes[0].first;
	vector<string> streams;
	for (size_t i = 0; i < keyLen; i++)
	{
		streams.push_back(string());
	}
	for(uint32_t i = 0; i < dataLow.size(); ++i)
	{
		streams[i%keyLen].push_back(dataLow.at(i));
	}
	vector<vector<pair<char, uint32_t>>> charFreqInStreams;
	for(uint8_t i = 0; i < keyLen; ++i)
	{
		cout << streams[i] << endl;
		charFreqInStreams.push_back(vector<pair<char, uint32_t>>());

		for (char curChar = 'a'; curChar <= 'z'; curChar++)
		{
			auto n = count(streams[i].begin(), streams[i].end(), curChar);
			charFreqInStreams.back().push_back(make_pair(curChar,n));
		}


		sort(charFreqInStreams[i].begin(), charFreqInStreams[i].end(),
			[](auto &a, auto &b) -> bool
		{
			return a.second > b.second;
		});

		for_each(charFreqInStreams[i].begin(), charFreqInStreams[i].begin() + 5, [](auto tmp) {
			cout << tmp.first << " " << tmp.second << '\n';
		});
	}

	vector<string> goodOrderChars;

	for each (auto tmp in charFreqInStreams)
	{
		auto lastChar = tmp.front().first;
		string rotStr;
		for_each(tmp.begin(), tmp.begin() + 5, [&rotStr](auto top5char)
		{
			rotStr.push_back(top5char.first);
		});
		for (int rotCnt = 0; rotCnt < 5; rotCnt++)
		{
			bool gotIt = false;
			auto hightChar = rotStr[0];
			for (int i = 0; i < 5; i++)
			{
				if ((rotStr[i] - hightChar) != ('t'-'e'))
				{
				}
				else
				{
					gotIt = true;
					break;
				}
			}
			if (gotIt)
			{
				break;
			} 
			else
			{
				rotate(rotStr.begin(), rotStr.begin() + 1, rotStr.end());
			}
		}


		cout << rotStr;
		goodOrderChars.push_back(rotStr);

		cout << '\n';
	}

	cout << '\n';
	

	//cin >> a;
    return 0;
}

