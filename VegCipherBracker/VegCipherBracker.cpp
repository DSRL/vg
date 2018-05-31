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
/**
 @def  MAXKEY
 @brief Sets the maximum key length
*/
#define MAXKEY 12
/**
@def  TESTCOUNT
@brief Sets number of test self test trials
*/
#define TESTCOUNT 10000
/**
@def  FASTTRY
@brief Speed up performance by risk of fail to decrypt (0.05%)
*/
#define FASTTRY
/**
@def  KEYLATTEMPTS
@brief Speed up performance by limiting key attempts for key size  risk of fail to decrypt under 3 (0.1%)
*/
#define KEYLATTEMPTS 3
/**
@def  debugLayer
@brief enum with debug layers
*/
enum debugLayer
{
	NoDEB,
	Light,
	Heavy
};

/*
* \var vector<string> popWords
* \brief vector of most popular English words
*/
vector<string> popWords
{
	"a",
	"and",
	"away",
	"big",
	"blue",
	"can",
	"come",
	"down",
	"find",
	"for",
	"funny",
	"go",
	"help",
	"here",
	"I",
	"in",
	"is",
	"it",
	"jump",
	"little",
	"look",
	"make",
	"me",
	"my",
	"not",
	"one",
	"play",
	"red",
	"run",
	"said",
	"see",
	"the",
	"three",
	"to",
	"two",
	"up",
	"we",
	"where",
	"yellow",
	"you",
	"all",
	"am",
	"are",
	"at",
	"ate",
	"be",
	"black",
	"brown",
	"but",
	"came",
	"did",
	"do",
	"eat",
	"four",
	"get",
	"good",
	"have",
	"he",
	"into",
	"like",
	"must",
	"new",
	"no",
	"now",
	"on",
	"our",
	"out",
	"please",
	"pretty",
	"ran",
	"ride",
	"saw",
	"say",
	"she",
	"so",
	"soon",
	"that",
	"there",
	"they",
	"this",
	"too",
	"under",
	"want",
	"was",
	"well",
	"went",
	"what",
	"white",
	"who",
	"will",
	"with",
	"yes",
	"after",
	"again",
	"an",
	"any",
	"as",
	"ask",
	"by",
	"could",
	"every",
	"fly",
	"from",
	"give",
	"giving",
	"had",
	"has",
	"her",
	"him",
	"his",
	"how",
	"just",
	"know",
	"let",
	"live",
	"may",
	"of",
	"old",
	"once",
	"open",
	"over",
	"put",
	"round",
	"some",
	"stop",
	"take",
	"thank",
	"them",
	"then",
	"think",
	"walk",
	"were",
	"when",
	"always",
	"around",
	"because",
	"been",
	"before",
	"best",
	"both",
	"buy",
	"call",
	"cold",
	"does",
	"don't",
	"fast",
	"first",
	"five",
	"found",
	"gave",
	"goes",
	"green",
	"its",
	"made",
	"many",
	"off",
	"or",
	"pull",
	"read",
	"right",
	"sing",
	"sit",
	"sleep",
	"tell",
	"their",
	"these",
	"those",
	"upon",
	"us",
	"use",
	"very",
	"wash",
	"which",
	"why",
	"wish",
	"work",
	"would",
	"write",
	"your",
	"about",
	"better",
	"bring",
	"carry",
	"clean",
	"cut",
	"done",
	"draw",
	"drink",
	"eight",
	"fall",
	"far",
	"full",
	"got",
	"grow",
	"hold",
	"hot",
	"hurt",
	"if",
	"keep",
	"kind",
	"laugh",
	"light",
	"long",
	"much",
	"myself",
	"never",
	"nine",
	"only",
	"own",
	"pick",
	"seven",
	"shall",
	"show",
	"six",
	"small",
	"start",
	"ten",
	"today",
	"together",
	"try",
	"warm",
	"apple",
	"baby",
	"back",
	"ball",
	"bear",
	"bed",
	"bell",
	"bird",
	"birthday",
	"boat",
	"box",
	"boy",
	"bread",
	"brother",
	"cake",
	"car",
	"cat",
	"chair",
	"chicken",
	"children",
	"Christmas",
	"coat",
	"corn",
	"cow",
	"day",
	"dog",
	"doll",
	"door",
	"duck",
	"egg",
	"eye",
	"farm",
	"farmer",
	"father",
	"feet",
	"fire",
	"fish",
	"floor",
	"flower",
	"game",
	"garden",
	"girl",
	"good-bye",
	"grass",
	"ground",
	"hand",
	"head",
	"hill",
	"home",
	"horse",
	"house",
	"kitty",
	"leg",
	"letter",
	"man",
	"men",
	"milk",
	"money",
	"morning",
	"mother",
	"name",
	"nest",
	"night",
	"paper",
	"party",
	"picture",
	"pig",
	"rabbit",
	"rain",
	"ring",
	"robin",
	"SantaClaus",
	"school",
	"seed",
	"sheep",
	"shoe",
	"sister",
	"snow",
	"song",
	"squirrel",
	"stick",
	"street",
	"sun",
	"table",
	"thing",
	"time",
	"top",
	"toy",
	"tree",
	"watch",
	"water",
	"way",
	"wind",
	"window",
	"wood"
};
/*
* \var map<char, double_t> EngFreq
* \brief map of English letters with probability chances
*/
map<char, double_t> EngFreq =
{
	{'a', 8.167},
	{'b', 1.492},
	{'c', 2.782},
	{'d', 4.253},
	{'e', 12.702},
	{'f', 2.228},
	{'g', 2.015},
	{'h', 6.094},
	{'i', 6.966},
	{'j', 0.153},
	{'k', 0.772},
	{'l', 4.025},
	{'m', 2.406},
	{'n', 6.749},
	{'o', 7.507},
	{'p', 1.929},
	{'q', 0.095},
	{'r', 5.987},
	{'s', 6.327},
	{'t', 9.056},
	{'u', 2.758},
	{'v', 0.978},
	{'w', 2.360},
	{'x', 0.150},
	{'y', 1.974},
	{'z', 0.074}
};

vector<string> findAllKeys(vector<vector<pair<char,double>>> const& dataVec)
{
	vector<string> retVec;
	int posCombinations = 1;
	for (auto tmp : dataVec)
	{
		posCombinations *= tmp.size();
	}
#ifdef FASTTRY
	if (posCombinations > 10) return retVec;
#else
	if (posCombinations > 100) return retVec;
#endif // FASTTRY

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
		//cout << endl;
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

vector<pair<uint8_t, double_t>> TrigramsTest(string encryptedMsg)
{
	vector<pair<uint8_t, double_t>> charChart;
	map<uint32_t, uint32_t> distances;

	for (auto i = 0; i < encryptedMsg.size(); i++)
	{
		auto keyStr = encryptedMsg.substr(i, 3);
		auto pos = encryptedMsg.find(keyStr, i);
		auto prevPos = pos;
		while (true)
		{
			pos = encryptedMsg.find(keyStr, ++pos);
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

		for_each(ordDistHits.begin(), ordDistHits.end(), [&](auto tmp) {
			//cout << tmp.first << " " << tmp.second << '\n';
			charChart.push_back(make_pair(tmp.first, tmp.second));
		});
	}
	else
	{
		cout << "Shift failed" << '\n';
	}

	//cout << '\n';
	return charChart;
}

vector<pair<uint8_t, double_t>>  AutoCorrelation(string encryptedMsg)
{
	vector<pair<uint8_t, double_t>> charChart;
	auto dataRot = encryptedMsg;
	auto pureSize = encryptedMsg.size();
	vector <pair<uint32_t, double_t>> ordDistHits;
	for (auto keyLen = 1; keyLen <= 12; keyLen++)
	{
		rotate(dataRot.begin(), dataRot.begin() + 1, dataRot.end());
		int counter = 0;
		for (auto i = 0; i < encryptedMsg.size(); i++)
		{
			if (encryptedMsg[i] == dataRot[i])
				counter++;
		}
		//cout << counter << "\t" << keyLen << "\t" << counter /(double)pureSize << endl;
		ordDistHits.push_back(make_pair(keyLen, counter / (double)pureSize));
	}

	sort(ordDistHits.begin(), ordDistHits.end(),
		[](auto &a, auto &b) -> bool
	{
		return a.second > b.second;
	});

	for_each(ordDistHits.begin(), ordDistHits.end(), [&](auto tmp) {
		//cout << tmp.first << " " << tmp.second << '\n';
		charChart.push_back(make_pair(tmp.first, tmp.second));
	});

	//cout << '\n';
	return charChart;
}

vector<pair<uint8_t, double_t>> CiCountTest(string encryptedMsg)
{
	vector<pair<uint8_t, double_t>> charChart;
	vector <pair<uint32_t, double_t>> ordDistHits;
	for (int keyLen = 1; keyLen <= MAXKEY; keyLen++)
	{
		double ciSum = 0;

		string tmp;
		for (auto i = encryptedMsg.begin();;)
		{
			//cout << *i;
			tmp += *i;
			if (i < encryptedMsg.end() - keyLen)
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

	for_each(ordDistHits.begin(), ordDistHits.end(), [&](auto tmp) {
		//cout << tmp.first << " " << tmp.second << '\n';
		charChart.push_back(make_pair(tmp.first, tmp.second));
	});
	return charChart;
}

/**
@brief Generated key string with specified size.

@param keyLen The lengths of key to generate.
@return generated key as string.
@throw string "Cant encrypt with 0 length key" if key is empty
*/
string generateKey(uint32_t keyLen)
{
	if (keyLen == 0) throw "Cant encrypt with 0 length key";
	string retStr;
	static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
	srand(time(0));
	for (int i = 0; i < keyLen; ++i) {
		retStr.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
	}
	return retStr;
}
/**
@brief Encrypt text with specified key.

@param key Key for encrypting text.
@param text Text to be encrypted by key.
@return encrypted text as string.
@throw string "Cant encrypt with empty key" if key is empty
@throw string "Cant encrypt  empty text" if text is empty
*/
string encrypt(string key, string text)
{
	if (key.empty()) throw "Cant encrypt with empty key";
	if (text.empty()) throw "Cant encrypt empty text";

	auto keyLen = key.size();
	string retStr;
	uint32_t curKeyChar = 0;

	for (uint32_t i = 0; i < text.size(); ++i)
	{
		if (regex_match(text.begin() + i, text.begin() + i + 1, regex("[a-z]")))
		{
			retStr.push_back((text[i] + (key[curKeyChar%keyLen] - 'a' - 'a')) % 26 + 'a');
			curKeyChar++;
		}
		else if (regex_match(text.begin() + i, text.begin() + i + 1, regex("[A-Z]")))
		{
			retStr.push_back((text[i] + (key[curKeyChar%keyLen] - 'a' - 'A')) % 26 + 'A');
			curKeyChar++;
		}
		else
		{
			retStr.push_back(text[i]);
		}
	}
	return retStr;
}
/**
@brief Dencrypt text with specified key.

@param key Key for dencrypting text.
@param text Text to be dencrypted by key.
@param origStr original Text with all symbols.
@return dencrypted text as string.
@throw string "Cant dencrypt with empty key" if key is empty
@throw string "Cant dencrypt  empty text" if text is empty
*/
string dencrypt(const string& key, const string& text, const string& origStr)
{
	if (key.empty()) throw "Cant dencrypt with empty key";
	if (text.empty()) throw "Cant dencrypt empty text";

	auto keyLen = key.size();
	auto plainText = text;

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
	return plainText;
}

/**
@brief Draw char frequency visualization.

@param charFreq data to for chart vector<pair<letter, ocurrancy>.
@return void.
@throw string "Cant draw with empty data" if charFreq is empty
*/
void drawFreqChart(const vector<pair<char, uint32_t>>& charFreq)
{
	if (charFreq.empty()) throw "Cant draw with empty data";

	cout << endl;
	cout << "Top 5 are:";
	for_each(charFreq.begin(), charFreq.begin() + 5, [](auto tmp)
	{
		cout <<" "<< tmp.first;
	});
	cout << endl;
	
	auto maxValueIt = max_element(charFreq.begin(), charFreq.end(), [](auto a, auto b) -> bool
	{
		return a.second < b.second;
	});
	auto maxVal = maxValueIt->second;
	//draw char colums
	for (size_t curCount = maxVal; curCount > 0; curCount--)
	{
		for_each(charFreq.begin(), charFreq.end(), [&curCount](auto tmp)
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

	//draw counter values
	for_each(charFreq.begin(), charFreq.end(), [](auto tmp)
	{
		cout << setfill(' ') << setw(2) << tmp.second << " ";
	});
	cout << endl;

	//draw characters
	for_each(charFreq.begin(), charFreq.end(), [](auto tmp)
	{
		cout << " " << tmp.first << " ";
	});
	cout << endl;
}

/**
@brief Run 3 Voters and get vote results.

@param encryptedMsg encrypted string for voters.
@return vector<pair<uint8_t, double_t>> result of 3 algorithm voters.
*/
vector<pair<uint8_t, double_t>> getKeyLenPoll(const string& encryptedMsg)
{
	vector<vector<pair<uint8_t, double_t>>> voters;

	voters.push_back(AutoCorrelation(encryptedMsg));
	voters.push_back(TrigramsTest(encryptedMsg));
	voters.push_back(CiCountTest(encryptedMsg));

	return voteForLen(voters);
}

/**
@brief trim text to only lower case chars
@param text Text to trimm.
@return void.
*/
void trimText(string& text)
{
	text = regex_replace(text, regex("[^A-Za-z]"), "");
	transform(text.begin(), text.end(), text.begin(), ::tolower);
}
/**
@brief Count popular words in string.
@param text text to look in.
@return uint32_t number of ocurensies.
*/
uint32_t countWordsInStr(const string& text)
{
	auto accCounter = 0;
	for_each(popWords.begin(), popWords.end(), [&](auto popWord)
	{
		int pos = text.find(popWord, 0);
		int prevPos;
		while (pos != string::npos)
		{
			pos = text.find(popWord, ++pos);
			accCounter++;
			prevPos = pos;
		}
	});
	return accCounter;
}
/**
@brief Looks for all possible chars for future possible keys.
@param charDispStore vector of all char counted from ecrypted text.
@return properly placed chars in correct order to form key(and their variants).
*/
vector<vector<pair<char, double>>> findAllCharForKey(const vector<vector<pair<char, uint32_t>>>& charDispStore)
{
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
		vector<pair<char, double>> rawCompareVec;
		auto statGap = charChance.back().front().second*1.1;
		for_each(charChance.back().begin(), charChance.back().end(), [&](auto tmp)
		{
			if (tmp.second > statGap) return;
			rawCompareVec.push_back(make_pair(tmp.first, tmp.second));
		});
		sort(rawCompareVec.begin(), rawCompareVec.end(),
			[](auto &a, auto &b) -> bool
		{
			return a.second > b.second;
		});

		goodOrderChars.push_back(rawCompareVec);
	});
	return goodOrderChars;
}
/**
@brief Count chars Occurrences in given text for each letter of key.
@param KeyLen estimated size of key.
@param encryptedMsg encrypted text.
@return properly placed chars in correct order to form key(and their variants).
*/
auto countCharOccurrences(const uint32_t KeyLen,const string& encryptedMsg)
{
	vector<string> streams;
	for (size_t i = 0; i < KeyLen; i++)
	{
		streams.push_back(string());
	}
	for (uint32_t i = 0; i < encryptedMsg.size(); ++i)
	{
		streams[i%KeyLen].push_back(encryptedMsg[i]);
	}

	vector<vector<pair<char, uint32_t>>> charDispStore;
	for (uint8_t i = 0; i < KeyLen; ++i)
	{
		charDispStore.push_back(vector<pair<char, uint32_t>>());

		for (char curChar = 'a'; curChar <= 'z'; curChar++)
		{
			auto n = count(streams[i].begin(), streams[i].end(), curChar);
			charDispStore.back().push_back(make_pair(curChar, n));
		}
	}
	return charDispStore;
}

int main()
{
	debugLayer dLayer = NoDEB;
	string input = "Decryption is performed by going to the row in the table corresponding to the key, finding the position of the ciphertext letter in this row, and then using the column’s label as the plaintext. For example, in row A (from AYUSH), the ciphertext G appears in column G, which is the first plaintext letter. Next we go to row Y (from AYUSH), locate the ciphertext C which is found in column E, thus E is the second plaintext letter.";
	//string input = "THISEMAILANDANYFILESTRANSMITTEDWITHITMAYCONTAINCONFIDENTIALINFORMATIONTHATISPRIVILEGEDOROTHERWISEEXEMPTFROMDISCLOSUREUNDERAPPLICABLELAWANYDISSEMINATIONDISTRIBUTIONORCOPYOFTHISCOMMUNICATIONOTHERTHANTOTHEINDIVIDUALORENTITYNAMEDABOVEISSTRICTLYPROHIBITEDIFYOUARENOTTHEINTENDEDADDRESSEENORAUTHORIZEDTORECEIVEFORTHEINTENDEDADDRESSEEYOUAREHEREBYNOTIFIEDTHATYOUMAYNOTUSECOPYDISCLOSETOANYONETHEINFORMATIONCONTAINEDINORATTACHEDTOTHISMESSAGEIFYOURECEIVEDTHISCOMMUNICATIONINERRORPLEASEIMMEDIATELYADVISEBYREPLYEMAILANDDELETETHISMESSAGEITSATTACHMENTSANDANYCOPIES";
	//string input = "The function template type can be any type of bidirectional iterator to characters.";
	
	for (int kl = 1; kl <= TESTCOUNT; kl++)
	{
		string startKey = generateKey(kl % MAXKEY + 1);
		
		string encryptedMsg = encrypt(startKey,input);

		auto origStr = encryptedMsg;

		switch (dLayer)
		{
		case Heavy:
			{
				auto origSize = encryptedMsg.size();
				encryptedMsg = regex_replace(encryptedMsg, regex("\\s+"), "");
				auto whitespaceSize = encryptedMsg.size();
				encryptedMsg = regex_replace(encryptedMsg, regex("\\d+"), "");
				auto numberSize = encryptedMsg.size();
				cout << encryptedMsg << endl;
				trimText(encryptedMsg);
				auto pureSize = encryptedMsg.size();
				cout << "origSize " << origSize << endl
					<< "whitespace " << origSize - whitespaceSize << endl
					<< "numbers " << whitespaceSize - numberSize << endl
					<< "restOfSumbols " << numberSize - pureSize << endl
					<< "pureSize " << pureSize << endl;
				cout << '\n';
				break;
			}
		case NoDEB:
		case Light:
		default:
			trimText(encryptedMsg);
			break;
		}

		auto PollRes = getKeyLenPoll(encryptedMsg);

		switch (dLayer)
		{
		case Light:
		case Heavy:
			for (auto var : PollRes)
			{
				cout << "keyLen is: " << static_cast<int>(var.first) << " " << var.second << endl;
				break;
			}
		case NoDEB:
		default:
			break;
		}
	
		/*uint8_t keyLen;
		int bestSize = 0;
		for (size_t i = 1; i < PollRes.size(); i++)
		{
			if (PollRes[i].second*1.25 > PollRes[0].second &&
				PollRes[i].first	  > PollRes[0].first  &&
				(!(PollRes[i].first%2 || PollRes[0].first%2)	||
				!(PollRes[i].first % 3 || PollRes[0].first % 3) ||
				!(PollRes[i].first % 5 || PollRes[0].first % 5) ||
				!(PollRes[i].first % 7)))
			{
				keyLen = PollRes[i].first;
				if (PollRes[bestSize].first<PollRes[i].first)
				bestSize = i;
			}
			else
			{
				keyLen = PollRes[bestSize].first;
			}
		}*/

#ifdef FASTTRY
		if (PollRes.size() > KEYLATTEMPTS) PollRes.resize(KEYLATTEMPTS);
#endif // FASTTRY

		vector<pair<string, int>> keyLenHits;
		for_each(PollRes.begin(),PollRes.end(),[&](auto keyLen)
		{
			auto charDispStore = countCharOccurrences(keyLen.first, encryptedMsg);

			for (uint8_t i = 0; i < keyLen.first; ++i)
			{
				switch (dLayer)
				{
				case Heavy:
					drawFreqChart(charDispStore[i]);
				case NoDEB:
				case Light:
				default:
					break;
				}
			}

			auto possibleKeys = findAllKeys(findAllCharForKey(charDispStore));
			
			if (possibleKeys.empty()) return;

			string plainText;
			vector<pair<string, int>> keyHits;
			for_each(possibleKeys.begin(), possibleKeys.end(), [&](auto key)
			{
				plainText = dencrypt(key, encryptedMsg, origStr);
				keyLenHits.push_back(make_pair(key, countWordsInStr(plainText)));
			});
		});
		//cout << "OursKey is:\t\t" << key;
		//cout << '\n';
		sort(keyLenHits.begin(), keyLenHits.end(), [](auto &a, auto &b) -> bool
		{
			return a.second > b.second;
		});
		for(auto curKey :keyLenHits)
		{
			auto key = curKey.first;
			string plainText = dencrypt(key, encryptedMsg, origStr);

			cout << plainText<< endl;
			static int wrongSize = 0;
			static int good = 0;
			static int nogood = 0;
			
			if (key.size() != startKey.size())
			{
				if (startKey.find(key) != std::string::npos)
				{
					cout << "Cesar" << good << ' ';
					cout << startKey<<" " << key << '\n';
				}
				else
				{
					cout << "good" << good << '\n';
					cout << "notgood" << nogood << '\n';
					cout << "wrongSize" << ++wrongSize << endl;
				}
			}
			if ((plainText.compare(input) == 0))
			{
				cout << "good" << ++good << '\n';
				cout << "notgood" << nogood << '\n';
				cout << "wrongSize" << wrongSize << endl;
				break;
			}
			else
			{
				if (curKey == keyLenHits.back())
				{
					cout << "good" << good << '\n';
					cout << "notgood" << ++nogood << '\n';
					cout << "wrongSize" << wrongSize << endl;
				}
			}
		}
	}

	//cin >> a;
    return 0;
}

