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
#include <ctime>
using namespace std;

#define MAXKEY 12
#define TESTCOUNT 10000

vector<string> findAllKeys(vector<vector<pair<char,double>>> const& dataVec)
{
	vector<string> retVec;
	int posCombinations = 1;
	for (auto tmp : dataVec)
	{
		posCombinations *= tmp.size();
	}

	int a;
	//cin >> a;
	for (size_t val = 0; val < posCombinations; val++)
	{
		string curWord;
		for (size_t i = 0; i < dataVec.size(); i++)
		{
			int mod = 1;
			for (size_t j = 0; j <= i; j++)
			{
				mod *= dataVec[j].size();
			}
			int dev = 1;
			for (size_t j = 0; j < i; j++)
			{
				dev *= dataVec[j].size();
			}
			//cout << dataVec[i][val%mod / dev].first << " ";
			curWord.push_back(dataVec[i][val%mod / dev].first);
		}
		for (auto i = 0; i < curWord.size(); i++)
		{
			curWord[i] = ('a' + (curWord[i] + 3) % 26);
		}
		retVec.push_back(curWord);
		cout << endl;
		//cout << val%ar[0] <<" "<< val % (ar[0]*ar[1]) / (ar[0]) << " " << val % (ar[0] * ar[1]* ar[2]) / (ar[0] * ar[1]) << endl;
	}
	return retVec;
}
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
		for (int i = 2; i <= MAXKEY; i++)
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
				//cout << tmp.first << " " << tmp.second << '\n';
				charChart.push_back(make_pair(tmp.first, tmp.second));
			});
		}
		else
		{
			for_each(ordDistHits.begin(), ordDistHits.end(), [&](auto tmp) {
				//cout << tmp.first << " " << tmp.second << '\n';
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
			//cout << tmp.first << " " << tmp.second << '\n';
			charChart.push_back(make_pair(tmp.first, tmp.second));
		});
	}
	else
	{
		for_each(ordDistHits.begin() + 1, ordDistHits.end(), [&](auto tmp) {
			//cout << tmp.first << " " << tmp.second << '\n';
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
	for (int keyLen = 1; keyLen <= MAXKEY; keyLen++)
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
			//cout << tmp.first << " " << tmp.second << '\n';
			charChart.push_back(make_pair(tmp.first, tmp.second));
		});
	}
	else
	{
		for_each(ordDistHits.begin() + 1, ordDistHits.end(), [&](auto tmp) {
			//cout << tmp.first << " " << tmp.second << '\n';
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
	///*required*/string input = "Klym qj e erwu wtrwv kicjtrxh klqn krr ev yiyl ks gvvypm zxhiejiz tpdjwum nisp zx. yn qj rrk ed cbvvdksh wtrwv rrt xwvw qfx flwmmgv eds ww xkv jkhkkmreebcbp eq zxuliksu zw urxvgwvh ji przh.klym jrwh tpqma frop thidzhhj wegm dipsih nggiv, nlywp zr irgj uzv rrk vukczvhu xe vm gvhjidn qe eqp mjyzrxri xojm(zxhiejiz kcsvw xudv rr jtuwqwmf dicvmi vhhyylmdiqkw), rob klhp qyapk fh lwuzcc, wlegu npvc gvjyhm klh dicvmiw qvityl wsu klu xmwexcx ynmiewfv_jlizxv tpqma kipgpqnm ks jvruliki wyi qjxissimqnm zrvkednqrxlfr qobfqdkmsutcc(deh ioky mqjxqhbzewzsd ca iitlmhyl ks ev zqfqu jri ebf qkiurxel bpthj).";
	///*BA*/string input = "Uhjs f-mbim aod bnz fjlfs urbntmjtuee wjti iu mby dootbio cpngieeotjam iofprnauipn uhbt js qrjvjlfgfd pr ptieswjsf eyenpu fson djsdlpsvrf uodfr bpqljcbbme max. Aoy eitsfmjnbtjoo, djsurjbvtjoo, os cppz og tiit cpmnuoidauipn pties tiao tp tie jneiwieubl pr fnuiuy oanee acowe js ttsidtmy qrphjbjtfd. Jf zov ase oou tie jnueodfd bderfstef, npr buuhprjzfd uo sedejvf fpr uhf iotfneee aedsetsfe, zov ase iesecy oouigifd uhbt zov mby oou ute, doqy, eitcmote uo bnzooe uhf iofprnauipn dootbioee io os autbciee tp tiit mfstahe. Jf zov rfcfiwee tiit cpmnuoidauipn jn frsos, pmebsf inmfdjauemy bdwite cy seqlz e-najl bne dflftf tiit mfstahe, jtt autbcimfnus bne aoy doqifs.";
	///*BAG*/string encryptedMsg = "UHOTESBIRBNJBNEGIRFSZSATTMOUTKEWOUHOUMGZCUOTGJNIPNLJDKOTOBLOOFUSMGUIUOTNBTOTPXJVOMEMFDUSOZIEXXIYFEDFMVUFXPMJJSIMOYVRKVNJFRGQPRJCGCLKMACBNEEIYTESJNGUIUODOTTXJBAUIUOOXDOVZOLUHOTCUNMAOIIBTOPNUUHKSTNBNZPTNFITEIBJDABLUSETUIZZNGNEJBBUWEOTSZSIIULEQRUIIHJTKEILZOABRKOOZUHKJNZFNJFDGEDXFSYFETPRGVTNPROAEJUOXFCKJVKGOXUHKJNZFNJFDGEDXFSYFEEPUGSENFRKCYTPTOGIKETNBTEPUSBYTPTATEIPPEEIYDLUTEZPATZOTFTNFITGOXNAZJOTDOTUAOOEJJNUSAZUAIIEJUOZIIYNEYTAMFILZOASEIFIBFDZIIYDOSNUTJCGUIUOITFRXPRVMEGTEONMKEIGUERZAJWIYFBESEVMYKNAOMATEDKMEZFTNJSSFSYBGKJTYBTZBCNNETUSGODGOYIPPOFS";
	///*SFQ*/string input = "Lmyk ni s wuendvjh lt khiqlj jg yxw qqljil aujxygs ex demw igkjofhw bxwwu jjbwaqfy jg flgnt sso htjwsjafb vnijzflnefx jg demw Hseuj jnhjhajduj. Ogz csd rw ficjt lt hwaywb emw dwb Jwwck fdv Hefiylnefx qk dem ziw tkj xexymswu sst kjhnnswx. Dwb lwwiatdk tv wcykyyfl igkjofhw bydq rw rqvj qnfydfrdj, imhx sx Hseuj Xofffkj 2.0 qfi 3.0. Vgw Hseuj Uxgsu gbdwwi, s suo zfvfjw mqk guws csiu saqaqqtqu onjz zfvfjwi jwwck.\n\
	Ye cjuh uquj mayx lmu fjm vfjs uhgyuuyygs bsbi lfaasw wkvwhj xwee Rqq 25, 2018 nd lmu Wzhguuss Kfnef, bu zflw zfvfjwi emw jwwck tv kjhnnsw fdv uhaaqud fgqyunuk.Bu wssgzhslu qtk lt hwaywb jzj dwb szfdyji lt emw fgqyunuk go nniayyfl emw Wwsujfb Vfjs Uhgyuuyygs Hwlkdfjatd(YIFJ) uqyj vgw cgwu asvgwcsyygs : xlyfk://bmo.wqrjh.utc/yifj\n\
	Bu sufjjsafjw demw imufgwj sst lwkky yf Wqrjh omuf ziasw gzh hwevzslx qfi iwwlahuk fdv xuwp ogzh cndv zdvjhkyqfiyfl qk bu utdlndmj jg jchtmww emw kkjhk ye efaw yxw guky twhyknefx qttkl yxw ndxthefjatd lmql ni kmqjjt onjz zi.";
	string input = "Decryption is performed by going to the row in the table corresponding to the key, finding the position of the ciphertext letter in this row, and then using the column’s label as the plaintext. For example, in row A (from AYUSH), the ciphertext G appears in column G, which is the first plaintext letter. Next we go to row Y (from AYUSH), locate the ciphertext C which is found in column E, thus E is the second plaintext letter.";
	//string input = "THISEMAILANDANYFILESTRANSMITTEDWITHITMAYCONTAINCONFIDENTIALINFORMATIONTHATISPRIVILEGEDOROTHERWISEEXEMPTFROMDISCLOSUREUNDERAPPLICABLELAWANYDISSEMINATIONDISTRIBUTIONORCOPYOFTHISCOMMUNICATIONOTHERTHANTOTHEINDIVIDUALORENTITYNAMEDABOVEISSTRICTLYPROHIBITEDIFYOUARENOTTHEINTENDEDADDRESSEENORAUTHORIZEDTORECEIVEFORTHEINTENDEDADDRESSEEYOUAREHEREBYNOTIFIEDTHATYOUMAYNOTUSECOPYDISCLOSETOANYONETHEINFORMATIONCONTAINEDINORATTACHEDTOTHISMESSAGEIFYOURECEIVEDTHISCOMMUNICATIONINERRORPLEASEIMMEDIATELYADVISEBYREPLYEMAILANDDELETETHISMESSAGEITSATTACHMENTSANDANYCOPIES";
	//kfnqduxw
	string origkey = "jkfasfgasfasklfjk";
	static const char alphanum[] =
		"abcdefghijklmnopqrstuvwxyz";


	for (int kl = 1; kl <= TESTCOUNT; kl++)
	{
		string startkey;
		//srand(time(0));
		for (int i = 0; i < kl % 12 + 1; ++i) {
			startkey.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
		}
		string encryptedMsg;
		uint32_t curKeyChar = 0;
		for (uint32_t i = 0; i < input.size(); ++i)
		{
			if (regex_match(&input[i], &input[i] + 1, regex("[a-z]")))
			{
				encryptedMsg.push_back((input[i] + (startkey[curKeyChar%startkey.size()] - 'a' - 'a')) % 26 + 'a');
				curKeyChar++;
			}
			else if (regex_match(&input[i], &input[i] + 1, regex("[A-Z]")))
			{
				encryptedMsg.push_back((input[i] + (startkey[curKeyChar%startkey.size()] - 'a' - 'A')) % 26 + 'A');
				curKeyChar++;
			}
			else
			{
				encryptedMsg.push_back(input[i]);
			}
		}

		vector<string> popWords;
		{
			popWords.push_back("a");
			popWords.push_back("and");
			popWords.push_back("away");
			popWords.push_back("big");
			popWords.push_back("blue");
			popWords.push_back("can");
			popWords.push_back("come");
			popWords.push_back("down");
			popWords.push_back("find");
			popWords.push_back("for");
			popWords.push_back("funny");
			popWords.push_back("go");
			popWords.push_back("help");
			popWords.push_back("here");
			popWords.push_back("I");
			popWords.push_back("in");
			popWords.push_back("is");
			popWords.push_back("it");
			popWords.push_back("jump");
			popWords.push_back("little");
			popWords.push_back("look");
			popWords.push_back("make");
			popWords.push_back("me");
			popWords.push_back("my");
			popWords.push_back("not");
			popWords.push_back("one");
			popWords.push_back("play");
			popWords.push_back("red");
			popWords.push_back("run");
			popWords.push_back("said");
			popWords.push_back("see");
			popWords.push_back("the");
			popWords.push_back("three");
			popWords.push_back("to");
			popWords.push_back("two");
			popWords.push_back("up");
			popWords.push_back("we");
			popWords.push_back("where");
			popWords.push_back("yellow");
			popWords.push_back("you");
			popWords.push_back("all");
			popWords.push_back("am");
			popWords.push_back("are");
			popWords.push_back("at");
			popWords.push_back("ate");
			popWords.push_back("be");
			popWords.push_back("black");
			popWords.push_back("brown");
			popWords.push_back("but");
			popWords.push_back("came");
			popWords.push_back("did");
			popWords.push_back("do");
			popWords.push_back("eat");
			popWords.push_back("four");
			popWords.push_back("get");
			popWords.push_back("good");
			popWords.push_back("have");
			popWords.push_back("he");
			popWords.push_back("into");
			popWords.push_back("like");
			popWords.push_back("must");
			popWords.push_back("new");
			popWords.push_back("no");
			popWords.push_back("now");
			popWords.push_back("on");
			popWords.push_back("our");
			popWords.push_back("out");
			popWords.push_back("please");
			popWords.push_back("pretty");
			popWords.push_back("ran");
			popWords.push_back("ride");
			popWords.push_back("saw");
			popWords.push_back("say");
			popWords.push_back("she");
			popWords.push_back("so");
			popWords.push_back("soon");
			popWords.push_back("that");
			popWords.push_back("there");
			popWords.push_back("they");
			popWords.push_back("this");
			popWords.push_back("too");
			popWords.push_back("under");
			popWords.push_back("want");
			popWords.push_back("was");
			popWords.push_back("well");
			popWords.push_back("went");
			popWords.push_back("what");
			popWords.push_back("white");
			popWords.push_back("who");
			popWords.push_back("will");
			popWords.push_back("with");
			popWords.push_back("yes");
			popWords.push_back("after");
			popWords.push_back("again");
			popWords.push_back("an");
			popWords.push_back("any");
			popWords.push_back("as");
			popWords.push_back("ask");
			popWords.push_back("by");
			popWords.push_back("could");
			popWords.push_back("every");
			popWords.push_back("fly");
			popWords.push_back("from");
			popWords.push_back("give");
			popWords.push_back("giving");
			popWords.push_back("had");
			popWords.push_back("has");
			popWords.push_back("her");
			popWords.push_back("him");
			popWords.push_back("his");
			popWords.push_back("how");
			popWords.push_back("just");
			popWords.push_back("know");
			popWords.push_back("let");
			popWords.push_back("live");
			popWords.push_back("may");
			popWords.push_back("of");
			popWords.push_back("old");
			popWords.push_back("once");
			popWords.push_back("open");
			popWords.push_back("over");
			popWords.push_back("put");
			popWords.push_back("round");
			popWords.push_back("some");
			popWords.push_back("stop");
			popWords.push_back("take");
			popWords.push_back("thank");
			popWords.push_back("them");
			popWords.push_back("then");
			popWords.push_back("think");
			popWords.push_back("walk");
			popWords.push_back("were");
			popWords.push_back("when");
			popWords.push_back("always");
			popWords.push_back("around");
			popWords.push_back("because");
			popWords.push_back("been");
			popWords.push_back("before");
			popWords.push_back("best");
			popWords.push_back("both");
			popWords.push_back("buy");
			popWords.push_back("call");
			popWords.push_back("cold");
			popWords.push_back("does");
			popWords.push_back("don't");
			popWords.push_back("fast");
			popWords.push_back("first");
			popWords.push_back("five");
			popWords.push_back("found");
			popWords.push_back("gave");
			popWords.push_back("goes");
			popWords.push_back("green");
			popWords.push_back("its");
			popWords.push_back("made");
			popWords.push_back("many");
			popWords.push_back("off");
			popWords.push_back("or");
			popWords.push_back("pull");
			popWords.push_back("read");
			popWords.push_back("right");
			popWords.push_back("sing");
			popWords.push_back("sit");
			popWords.push_back("sleep");
			popWords.push_back("tell");
			popWords.push_back("their");
			popWords.push_back("these");
			popWords.push_back("those");
			popWords.push_back("upon");
			popWords.push_back("us");
			popWords.push_back("use");
			popWords.push_back("very");
			popWords.push_back("wash");
			popWords.push_back("which");
			popWords.push_back("why");
			popWords.push_back("wish");
			popWords.push_back("work");
			popWords.push_back("would");
			popWords.push_back("write");
			popWords.push_back("your");
			popWords.push_back("about");
			popWords.push_back("better");
			popWords.push_back("bring");
			popWords.push_back("carry");
			popWords.push_back("clean");
			popWords.push_back("cut");
			popWords.push_back("done");
			popWords.push_back("draw");
			popWords.push_back("drink");
			popWords.push_back("eight");
			popWords.push_back("fall");
			popWords.push_back("far");
			popWords.push_back("full");
			popWords.push_back("got");
			popWords.push_back("grow");
			popWords.push_back("hold");
			popWords.push_back("hot");
			popWords.push_back("hurt");
			popWords.push_back("if");
			popWords.push_back("keep");
			popWords.push_back("kind");
			popWords.push_back("laugh");
			popWords.push_back("light");
			popWords.push_back("long");
			popWords.push_back("much");
			popWords.push_back("myself");
			popWords.push_back("never");
			popWords.push_back("nine");
			popWords.push_back("only");
			popWords.push_back("own");
			popWords.push_back("pick");
			popWords.push_back("seven");
			popWords.push_back("shall");
			popWords.push_back("show");
			popWords.push_back("six");
			popWords.push_back("small");
			popWords.push_back("start");
			popWords.push_back("ten");
			popWords.push_back("today");
			popWords.push_back("together");
			popWords.push_back("try");
			popWords.push_back("warm");
			popWords.push_back("apple");
			popWords.push_back("baby");
			popWords.push_back("back");
			popWords.push_back("ball");
			popWords.push_back("bear");
			popWords.push_back("bed");
			popWords.push_back("bell");
			popWords.push_back("bird");
			popWords.push_back("birthday");
			popWords.push_back("boat");
			popWords.push_back("box");
			popWords.push_back("boy");
			popWords.push_back("bread");
			popWords.push_back("brother");
			popWords.push_back("cake");
			popWords.push_back("car");
			popWords.push_back("cat");
			popWords.push_back("chair");
			popWords.push_back("chicken");
			popWords.push_back("children");
			popWords.push_back("Christmas");
			popWords.push_back("coat");
			popWords.push_back("corn");
			popWords.push_back("cow");
			popWords.push_back("day");
			popWords.push_back("dog");
			popWords.push_back("doll");
			popWords.push_back("door");
			popWords.push_back("duck");
			popWords.push_back("egg");
			popWords.push_back("eye");
			popWords.push_back("farm");
			popWords.push_back("farmer");
			popWords.push_back("father");
			popWords.push_back("feet");
			popWords.push_back("fire");
			popWords.push_back("fish");
			popWords.push_back("floor");
			popWords.push_back("flower");
			popWords.push_back("game");
			popWords.push_back("garden");
			popWords.push_back("girl");
			popWords.push_back("good-bye");
			popWords.push_back("grass");
			popWords.push_back("ground");
			popWords.push_back("hand");
			popWords.push_back("head");
			popWords.push_back("hill");
			popWords.push_back("home");
			popWords.push_back("horse");
			popWords.push_back("house");
			popWords.push_back("kitty");
			popWords.push_back("leg");
			popWords.push_back("letter");
			popWords.push_back("man");
			popWords.push_back("men");
			popWords.push_back("milk");
			popWords.push_back("money");
			popWords.push_back("morning");
			popWords.push_back("mother");
			popWords.push_back("name");
			popWords.push_back("nest");
			popWords.push_back("night");
			popWords.push_back("paper");
			popWords.push_back("party");
			popWords.push_back("picture");
			popWords.push_back("pig");
			popWords.push_back("rabbit");
			popWords.push_back("rain");
			popWords.push_back("ring");
			popWords.push_back("robin");
			popWords.push_back("SantaClaus");
			popWords.push_back("school");
			popWords.push_back("seed");
			popWords.push_back("sheep");
			popWords.push_back("shoe");
			popWords.push_back("sister");
			popWords.push_back("snow");
			popWords.push_back("song");
			popWords.push_back("squirrel");
			popWords.push_back("stick");
			popWords.push_back("street");
			popWords.push_back("sun");
			popWords.push_back("table");
			popWords.push_back("thing");
			popWords.push_back("time");
			popWords.push_back("top");
			popWords.push_back("toy");
			popWords.push_back("tree");
			popWords.push_back("watch");
			popWords.push_back("water");
			popWords.push_back("way");
			popWords.push_back("wind");
			popWords.push_back("window");
			popWords.push_back("wood");
		}
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


		auto origStr = encryptedMsg;
		auto origSize = encryptedMsg.size();
		encryptedMsg = regex_replace(encryptedMsg, regex("\\s+"), "");
		auto whitespaceSize = encryptedMsg.size();
		encryptedMsg = regex_replace(encryptedMsg, regex("\\d+"), "");
		auto numberSize = encryptedMsg.size();
		encryptedMsg = regex_replace(encryptedMsg, regex("[^A-Za-z]"), "");
		auto pureSize = encryptedMsg.size();
		/*cout << encryptedMsg << endl;
		cout << "origSize " << origSize << endl
			<< "whitespace " << origSize - whitespaceSize << endl
			<< "numbers " << whitespaceSize - numberSize << endl
			<< "restOfSumbols " << numberSize - pureSize << endl
			<< "pureSize " << pureSize << endl;
		cout << '\n';*/
		auto dataLow = encryptedMsg;
		transform(dataLow.begin(), dataLow.end(), dataLow.begin(), ::tolower);
		vector<vector<pair<uint8_t, double_t>>> voters;
		voters.push_back(AutoCorrelation(dataLow));
		voters.push_back(TrigramsTest(dataLow));
		voters.push_back(CiCountTest(dataLow));

		auto PollRes = voteForLen(voters);

		for (auto var : PollRes)
		{
			//cout << "keyLen is: " << static_cast<int>(var.first) << " " << var.second << endl;
		}
		uint8_t keyLen = PollRes[0].first;

		//cout << '\n';
		//cout << "\nGoodKey is:\t\t" << startkey << endl;
		static int wrongSize = 0;
		static int good = 0;
		static int nogood = 0;
		if (keyLen != startkey.size())
		{
			if (startkey.size() == 1)
			{
				cout << "Cesar" << good << '\n';
			}
			else
			{
				cout << "good" << good << '\n';
				cout << "notgood" << ++nogood << '\n';
				cout << "wrongSize" << ++wrongSize << endl;
				continue;
			}
		}

		vector<string> streams;
		for (size_t i = 0; i < keyLen; i++)
		{
			streams.push_back(string());
		}
		for (uint32_t i = 0; i < dataLow.size(); ++i)
		{
			streams[i%keyLen].push_back(dataLow.at(i));
		}
		vector<vector<pair<char, uint32_t>>> charFreqInStreams;
		vector<vector<pair<char, uint32_t>>> charDispStore;
		for (uint8_t i = 0; i < keyLen; ++i)
		{
			//cout << streams[i] << endl;
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

			/*for_each(charFreqInStreams[i].begin(), charFreqInStreams[i].begin() + 5, [](auto tmp)
			{
				cout << " " << tmp.first << " ";
			});*/


	/*
			for (size_t curCount = charFreqInStreams[0][0].second; curCount > 0; curCount--)
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
				cout << setfill(' ') << setw(2) << tmp.second << " ";
			});
			cout << endl;
			for_each(charDispStore[i].begin(), charDispStore[i].end(), [](auto tmp)
			{
				cout << " " << tmp.first << " ";
			});

			cout << endl;*/
		}

		vector<vector<pair<char, double>>> goodOrderChars;


		vector<vector<pair<char, double>>> charChance;

		for_each(charDispStore.begin(), charDispStore.end(), [&](auto substr)
		{
			auto origOrderStr = substr;
			auto sum = 0.0;
			charChance.push_back(vector<pair<char, double>>());
			for_each(substr.begin(), substr.end(), [&](auto tmp)
			{
				sum += tmp.second;
			});
			sum /= 100.0;
			for (char i = 0; i <= 'z' - 'a'; i++)
			{
				auto difSum = 0.0;
				char counter = 'a';
				rotate(substr.begin(), substr.begin() + 1, substr.end());
				//cout << substr['e' - 'a'].first << '\n';
				for_each(substr.begin(), substr.end(), [&](auto ch)
				{
					//cout << (ch.second / sum) << "\t" << EngFreq[counter]<<endl;
					difSum += abs((ch.second / sum) - EngFreq[counter++]);
				});
				//cout << '\n';
				charChance.back().push_back(make_pair(substr['e' - 'a'].first, difSum));
			}
			sort(charChance.back().begin(), charChance.back().end(),
				[](auto &a, auto &b) -> bool
			{
				return a.second < b.second;
			});
			map<char, double_t> bestChars;
			auto statGap = charChance.back().front().second*1.1;
			for_each(charChance.back().begin(), charChance.back().end(), [&](auto tmp)
			{
				if (tmp.second > statGap) return;
				bestChars[tmp.first] = tmp.second;
			});
			vector<pair<char, double>> rawCompareVec;
			for_each(bestChars.begin(), bestChars.end(), [&](auto tmp)
			{
				rawCompareVec.push_back(make_pair(tmp.first, tmp.second));
			});
			sort(rawCompareVec.begin(), rawCompareVec.end(),
				[](auto &a, auto &b) -> bool
			{
				return a.second > b.second;
			});
			//cout<< charChance.back().front().first << '\n';
			goodOrderChars.push_back(rawCompareVec);
		});

		auto possibleKeys = findAllKeys(goodOrderChars);


		string plainText;
		vector<pair<string, int>> keyHits;
		for_each(possibleKeys.begin(), possibleKeys.end(), [&](auto key)
		{
			plainText = dataLow;

			for (int codeChar = 0; codeChar < plainText.length(); codeChar++)
				plainText[codeChar] = (char)(((plainText[codeChar] - key[codeChar%keyLen] + 26) % 26) + 'a');

			for (int i = 0; i < origStr.size(); i++)
			{
				if (regex_match(&origStr[i], &origStr[i] + 1, regex("[A-Z]")))
				{
					plainText[i] = toupper(plainText[i]);
				}
				else if (!regex_match(&origStr[i], &origStr[i] + 1, regex("[a-z]")))
				{
					plainText.insert(i, 1, origStr[i]);
				}
			}

			int accCounter = 0;
			for_each(popWords.begin(), popWords.end(), [&](auto popWord) 
			{
				int pos = plainText.find(popWord, 0);
				int prevPos;
				while (pos != string::npos)
				{
					pos = plainText.find(popWord, ++pos);
					accCounter++;
					prevPos = pos;
				}
			});
			keyHits.push_back(make_pair(key,accCounter));
		});

		auto goodkeyuIt = max_element(keyHits.begin(), keyHits.end(), [](auto &a, auto &b) -> bool
		{
			return a.second < b.second;
		});
		auto key = goodkeyuIt->first;
	
		//cout << "OursKey is:\t\t" << key;
		//cout << '\n';
		{
			plainText = dataLow;

			for (int codeChar = 0; codeChar < plainText.length(); codeChar++)
				plainText[codeChar] = (char)(((plainText[codeChar] - key[codeChar%keyLen] + 26) % 26) + 'a');

			for (int i = 0; i < origStr.size(); i++)
			{
				if (regex_match(&origStr[i], &origStr[i] + 1, regex("[A-Z]")))
				{
					plainText[i] = toupper(plainText[i]);
				}
				else if (!regex_match(&origStr[i], &origStr[i] + 1, regex("[a-z]")))
				{
					plainText.insert(i, 1, origStr[i]);
				}
			}
		}
		if ((plainText.compare(input) == 0))
		{
			cout << "good" << ++good << '\n';
			cout << "notgood" << nogood << '\n';
			cout << "wrongSize" << wrongSize << endl;
		}
		else
		{
			cout << "good" << good << '\n';
			cout << "notgood" << ++nogood << '\n';
			cout << "wrongSize" << wrongSize << endl;
		}
	}

	//cin >> a;
    return 0;
}

