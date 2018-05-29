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
#include <iomanip>
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

	for (auto i = 0; i < dataLow.size(); i++)
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
			double cn = static_cast<double>(n * (n - 1));
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
	/*required*/string input = "Klym qj e erwu wtrwv kicjtrxh klqn krr ev yiyl ks gvvypm zxhiejiz tpdjwum nisp zx. yn qj rrk ed cbvvdksh wtrwv rrt xwvw qfx flwmmgv eds ww xkv jkhkkmreebcbp eq zxuliksu zw urxvgwvh ji przh.klym jrwh tpqma frop thidzhhj wegm dipsih nggiv, nlywp zr irgj uzv rrk vukczvhu xe vm gvhjidn qe eqp mjyzrxri xojm(zxhiejiz kcsvw xudv rr jtuwqwmf dicvmi vhhyylmdiqkw), rob klhp qyapk fh lwuzcc, wlegu npvc gvjyhm klh dicvmiw qvityl wsu klu xmwexcx ynmiewfv_jlizxv tpqma kipgpqnm ks jvruliki wyi qjxissimqnm zrvkednqrxlfr qobfqdkmsutcc(deh ioky mqjxqhbzewzsd ca iitlmhyl ks ev zqfqu jri ebf qkiurxel bpthj).";
	///*BA*/string input = "Uhjs f-mbim aod bnz fjlfs urbntmjtuee wjti iu mby dootbio cpngieeotjam iofprnauipn uhbt js qrjvjlfgfd pr ptieswjsf eyenpu fson djsdlpsvrf uodfr bpqljcbbme max. Aoy eitsfmjnbtjoo, djsurjbvtjoo, os cppz og tiit cpmnuoidauipn pties tiao tp tie jneiwieubl pr fnuiuy oanee acowe js ttsidtmy qrphjbjtfd. Jf zov ase oou tie jnueodfd bderfstef, npr buuhprjzfd uo sedejvf fpr uhf iotfneee aedsetsfe, zov ase iesecy oouigifd uhbt zov mby oou ute, doqy, eitcmote uo bnzooe uhf iofprnauipn dootbioee io os autbciee tp tiit mfstahe. Jf zov rfcfiwee tiit cpmnuoidauipn jn frsos, pmebsf inmfdjauemy bdwite cy seqlz e-najl bne dflftf tiit mfstahe, jtt autbcimfnus bne aoy doqifs.";
	///*BAG*/string input = "UHOTESBIRBNJBNEGIRFSZSATTMOUTKEWOUHOUMGZCUOTGJNIPNLJDKOTOBLOOFUSMGUIUOTNBTOTPXJVOMEMFDUSOZIEXXIYFEDFMVUFXPMJJSIMOYVRKVNJFRGQPRJCGCLKMACBNEEIYTESJNGUIUODOTTXJBAUIUOOXDOVZOLUHOTCUNMAOIIBTOPNUUHKSTNBNZPTNFITEIBJDABLUSETUIZZNGNEJBBUWEOTSZSIIULEQRUIIHJTKEILZOABRKOOZUHKJNZFNJFDGEDXFSYFETPRGVTNPROAEJUOXFCKJVKGOXUHKJNZFNJFDGEDXFSYFEEPUGSENFRKCYTPTOGIKETNBTEPUSBYTPTATEIPPEEIYDLUTEZPATZOTFTNFITGOXNAZJOTDOTUAOOEJJNUSAZUAIIEJUOZIIYNEYTAMFILZOASEIFIBFDZIIYDOSNUTJCGUIUOITFRXPRVMEGTEONMKEIGUERZAJWIYFBESEVMYKNAOMATEDKMEZFTNJSSFSYBGKJTYBTZBCNNETUSGODGOYIPPOFS";
	///*SFQ*/string input = "Lmyk ni s wuendvjh lt khiqlj jg yxw qqljil aujxygs ex demw igkjofhw bxwwu jjbwaqfy jg flgnt sso htjwsjafb vnijzflnefx jg demw Hseuj jnhjhajduj. Ogz csd rw ficjt lt hwaywb emw dwb Jwwck fdv Hefiylnefx qk dem ziw tkj xexymswu sst kjhnnswx. Dwb lwwiatdk tv wcykyyfl igkjofhw bydq rw rqvj qnfydfrdj, imhx sx Hseuj Xofffkj 2.0 qfi 3.0. Vgw Hseuj Uxgsu gbdwwi, s suo zfvfjw mqk guws csiu saqaqqtqu onjz zfvfjwi jwwck.\n\
	Ye cjuh uquj mayx lmu fjm vfjs uhgyuuyygs bsbi lfaasw wkvwhj xwee Rqq 25, 2018 nd lmu Wzhguuss Kfnef, bu zflw zfvfjwi emw jwwck tv kjhnnsw fdv uhaaqud fgqyunuk.Bu wssgzhslu qtk lt hwaywb jzj dwb szfdyji lt emw fgqyunuk go nniayyfl emw Wwsujfb Vfjs Uhgyuuyygs Hwlkdfjatd(YIFJ) uqyj vgw cgwu asvgwcsyygs : xlyfk://bmo.wqrjh.utc/yifj\n\
	Bu sufjjsafjw demw imufgwj sst lwkky yf Wqrjh omuf ziasw gzh hwevzslx qfi iwwlahuk fdv xuwp ogzh cndv zdvjhkyqfiyfl qk bu utdlndmj jg jchtmww emw kkjhk ye efaw yxw guky twhyknefx qttkl yxw ndxthefjatd lmql ni kmqjjt onjz zi.";

	map<char, double_t> EngFreq;
	{
		EngFreq['a'] = 8.167;
		EngFreq['b'] = 1.492;
		EngFreq['c'] = 2.782;
		EngFreq['d'] = 4.253;
		EngFreq['e'] = 12.702;
		EngFreq['f'] = 2.228;
		EngFreq['g'] = 2.015;
		EngFreq['h'] = 6.094;
		EngFreq['i'] = 6.966;
		EngFreq['j'] = 0.153;
		EngFreq['k'] = 0.772;
		EngFreq['l'] = 4.025;
		EngFreq['m'] = 2.406;
		EngFreq['n'] = 6.749;
		EngFreq['o'] = 7.507;
		EngFreq['p'] = 1.929;
		EngFreq['q'] = 0.095;
		EngFreq['r'] = 5.987;
		EngFreq['s'] = 6.327;
		EngFreq['t'] = 9.056;
		EngFreq['u'] = 2.758;
		EngFreq['v'] = 0.978;
		EngFreq['w'] = 2.360;
		EngFreq['x'] = 0.150;
		EngFreq['y'] = 1.974;
		EngFreq['z'] = 0.074;
	}


	auto origStr = input;
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

	for (auto var : PollRes)
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
	vector<vector<pair<char, uint32_t>>> charDispStore;
	for(uint8_t i = 0; i < keyLen; ++i)
	{
		cout << streams[i] << endl;
		charFreqInStreams.push_back(vector<pair<char, uint32_t>>());
		charDispStore.push_back(vector<pair<char, uint32_t>>());

		for (char curChar = 'a'; curChar <= 'z'; curChar++)
		{
			auto n = count(streams[i].begin(), streams[i].end(), curChar);
			charFreqInStreams.back().push_back(make_pair(curChar, n));
			charDispStore.back().push_back(make_pair(curChar, n));
		}

		sort(charFreqInStreams[i].begin(), charFreqInStreams[i].end(),
			[](auto &a, auto &b) -> bool
		{
			return a.second > b.second;
		});

		for_each(charFreqInStreams[i].begin(), charFreqInStreams[i].begin()+5, [](auto tmp)
		{
			cout << " " << tmp.first << " ";
		});



		for (size_t curCount = charFreqInStreams[0][0].second; curCount > 0;curCount--)
		{
			for_each(charDispStore[i].begin(), charDispStore[i].end(), [&curCount](auto tmp)
			{
				if (tmp.second >= curCount)
				{
					cout << " | ";
				}
				else
				{
					cout << "   ";
				}
			});
			cout << endl;
		}
		for_each(charDispStore[i].begin(), charDispStore[i].end(), [](auto tmp)
		{
			cout << setfill(' ') << setw(2) << tmp.second<<" ";
		});
		cout << endl;
		for_each(charDispStore[i].begin(), charDispStore[i].end(), [](auto tmp)
		{
			cout<<" " << tmp.first << " ";
		});

		cout << endl;
	}

	string goodOrderChars;


	vector<vector<pair<char, double>>> charChance;

	for_each (charDispStore.begin(), charDispStore.end(),[&](auto substr)
	{	
		auto origOrderStr = substr;
		auto sum = 0.0;
		charChance.push_back(vector<pair<char, double>>());
		for_each(substr.begin(), substr.end(), [&](auto tmp)
		{
			sum += tmp.second;
		});
		sum  /= 100.0;
		for (char i = 0; i <= 'z' - 'a'; i++)
		{
			auto difSum = 0.0;
			char counter = 'a';
			rotate(substr.begin(), substr.begin()+1, substr.end());
			//cout << substr['e' - 'a'].first << '\n';
			for_each(substr.begin(), substr.end(), [&](auto ch)
			{
				//cout << (ch.second / sum) << "\t" << EngFreq[counter]<<endl;
				difSum += abs((ch.second/sum) - EngFreq[counter++]);
			});
			//cout << '\n';
			charChance.back().push_back(make_pair(substr['e'-'a'].first,difSum));
		}
		sort(charChance.back().begin(), charChance.back().end(),
			[](auto &a, auto &b) -> bool
		{
			return a.second < b.second;
		});
		map<char, double_t> bestChars;
		auto statGap = charChance.back().front().second*1.1;
		for_each(charChance.back().begin(), charChance.back().end(),[&](auto tmp)
		{
			if (tmp.second > statGap) return;
			bestChars[tmp.first] = tmp.second;
		});	
		vector<pair<char, int>> rawCompareVec;
		for_each(bestChars.begin(), bestChars.end(), [&](auto tmp)
		{
			auto tmpOrig = origOrderStr;
			rawCompareVec.push_back(make_pair(tmp.first, 0));
			rotate(tmpOrig.begin(), tmpOrig.begin() + (tmp.first%26), tmpOrig.end());
			if (tmpOrig[('e' - 'a')].second / sum > (EngFreq['e'] * 0.8))
				rawCompareVec.back().second++;
			if (tmpOrig[('t' - 'a')].second / sum > (EngFreq['t'] * 0.8))
				rawCompareVec.back().second++;
			if (tmpOrig[('a' - 'a')].second / sum > (EngFreq['a'] * 0.8))
				rawCompareVec.back().second++;
			if (tmpOrig[('o' - 'a')].second / sum > (EngFreq['o'] * 0.8))
				rawCompareVec.back().second++;
			if (tmpOrig[('i' - 'a')].second / sum > (EngFreq['i'] * 0.8))
				rawCompareVec.back().second++;
			if (tmpOrig[('j' - 'a')].second / sum < 4)
				rawCompareVec.back().second++;
			if (tmpOrig[('x' - 'a')].second / sum < 4)
				rawCompareVec.back().second++;
			if (tmpOrig[('z' - 'a')].second / sum < 4)
				rawCompareVec.back().second++;
			if (tmpOrig[('q' - 'a')].second / sum < 4)
				rawCompareVec.back().second++;
		});
		sort(rawCompareVec.begin(), rawCompareVec.end(),
			[](auto &a, auto &b) -> bool
		{
			return a.second > b.second;
		});
		//cout<< charChance.back().front().first << '\n';
		goodOrderChars.push_back(rawCompareVec.front().first);
	});
	
	string key;
	cout << "\npreKey is:\t";
	for (auto i = 0; i < keyLen; i++)
	{
		cout << goodOrderChars[i];
	}
	cout << '\n';
	for (auto i = 0; i < keyLen; i++)
	{

		key.push_back('a' + (goodOrderChars[i]+3)%26);
		cout<<(uint32_t)key.back() << '\n';
	}
	cout << '\n';
	cout << "Key is:\t\t" << key;
	cout << "\nKey is:\t\t" << "required";
	cout << '\n';
	cout << '\n';
	string plainText = dataLow;
	for (int keyChar = 0; keyChar<keyLen; keyChar++)
	{
		for (int codeChar = keyChar; codeChar<plainText.length(); codeChar += keyLen)
			plainText[codeChar] = (char)((plainText[codeChar] - key[keyChar] + 26) % 26 + 'a');
	}

	for (int i = 0; i < origStr.size(); i++ )
	{		
		//if (i > 959) cout << plainText;
		if (isupper(origStr[i]))
		{
			plainText[i] = toupper(plainText[i]);
		}
		else if (!regex_match(&origStr[i], &origStr[i] + 1, regex("[a-z]")))
		{
			plainText.insert(i, 1, origStr[i]);
		}
	}

	cout << plainText << "\n";

	//cin >> a;
    return 0;
}

