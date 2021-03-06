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
#include <iostream>
#include <fstream>

using namespace std;
/**
 @def  MAXKEY
 @brief Sets the maximum key length
*/
#define MAXKEY 12
/**
@def  AUTOTEST
@brief Let soft do test in auto mode
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

vector<string> findAllKeys(vector<vector<pair<char,double>>> const& dataVec, bool fast = true)
{
	vector<string> retVec;
	int posCombinations = 1;
	for (auto tmp : dataVec)
	{
		posCombinations *= tmp.size();
	}

	if (fast)
		if (posCombinations > 10) return retVec;
	else
		if (posCombinations > 100) return retVec;

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
	{
		plainText[codeChar] = (char)(((plainText[codeChar] - key[codeChar%keyLen] + 26) % 26) + 'a');
	}
	if (!origStr.empty())
	{
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
@param dLayer debugLayer on/off debug/info messages with voting results.
@return vector<pair<uint8_t, double_t>> result of 3 algorithm voters.
*/
vector<pair<uint8_t, double_t>> getKeyLenPoll(const string& encryptedMsg,debugLayer dLayer = NoDEB )
{
	vector<vector<pair<uint8_t, double_t>>> voters;

	voters.push_back(AutoCorrelation(encryptedMsg));
	voters.push_back(TrigramsTest(encryptedMsg));
	voters.push_back(CiCountTest(encryptedMsg));
	auto PollRes = voteForLen(voters);
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
	return voteForLen(voters);
}

/**
@brief trim text to only lower case chars
@param text Text to trim.
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
@return uint32_t number of occurrences.
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
@param charDispStore vector of all char counted from encrypted text.
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
vector<vector<pair<char, uint32_t>>> countCharOccurrences(const uint32_t KeyLen,const string& encryptedMsg)
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
/**
@brief Get all possible keys and their chance statistics.
@param KeyLenArray estimated size of key.
@param encryptedMsg encrypted text.
@param dLayer debugLayer on/off debug/info messages with voting results.
@return vector<pair<string, int>>  all possible keys and their chance statistics.
*/
vector<pair<string, int>> getAllPossibleKeys(const vector<pair<uint8_t, double_t>>& KeyLenArray,string encryptedMsg, debugLayer dLayer = NoDEB, bool fast = true)
{
	vector<pair<string, int>> keyLenHits;
	for_each(KeyLenArray.begin(), KeyLenArray.end(), [&](auto keyLen)
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

		auto possibleKeys = findAllKeys(findAllCharForKey(charDispStore),fast);

		if (possibleKeys.empty()) return;

		string plainText;
		vector<pair<string, int>> keyHits;
		for_each(possibleKeys.begin(), possibleKeys.end(), [&](auto key)
		{
			plainText = dencrypt(key, encryptedMsg, string());
			keyLenHits.push_back(make_pair(key, countWordsInStr(plainText)));
		});
	});
	//cout << "OursKey is:\t\t" << key;
	//cout << '\n';
	sort(keyLenHits.begin(), keyLenHits.end(), [](auto &a, auto &b) -> bool
	{
		return a.second > b.second;
	});
	if (dLayer == Light) cout << "OursKey is:\t\t" << keyLenHits.begin()->first << endl;
	return keyLenHits;
}
/**
@brief attack on trimmed text encoded by Vegener cypher.
@return vector<pair<string, int>>  all possible keys and their chance statistics.
*/
vector<pair<string, int>> attackOnVegenere(string encryptedMsg, debugLayer dlayer= NoDEB, bool fast = true)
{
	trimText(encryptedMsg);
	auto PollRes = getKeyLenPoll(encryptedMsg);

	if ((PollRes.size() > KEYLATTEMPTS) && fast) PollRes.resize(KEYLATTEMPTS);

	return getAllPossibleKeys(PollRes, encryptedMsg, dlayer,fast);
}
/**
@brief print help message to console.
*/
void printHelp()
{
	cout << "Type -selftest \"n\" to run self test with random key (2-12 key len) n-times(default 10000 max 99999999) -m \"0-1\" to select manual check mode(1) or auto(0)(default)" << endl;
	cout << endl;
	cout << "Select input source:" << endl;
	cout << "  Type -console-input-enc \"text\" use console-input text for encryption -key parameter is a mast" << endl;
	cout << "  Type -file \"file dir\" use selected file for encryption -key parameter is a mast" << endl;
	cout << "  Type -console-input-dec \"encrypted text\" use console-input text for decryption" << endl;
	cout << "  Type -efile \"encrypted file dir\" use selected file for decryption " << endl;
	cout << endl;
	cout << "Select output target:" << endl;
	cout << "  Type -dfile \"file dir\" result will be printed to selected file" << endl;
	cout << "  Type -console-output result will be printed to console" << endl;
	cout << endl;
	cout << "Additional params:" << endl;
	cout << "  -key Type  -key \"key\" to use known key for operation else attack on cypher will be used " << endl;
	cout << "  -trim \"0 or 1\" type \"-trim 0\" to keep non char symbols in encoded text, only for encryption (default 0)" << endl;
	cout << "  -fast With this option select fast(default) or slow algorithm to be used (fast may fail in rare conditions) -fast \"0 or 1\"" << endl;
	cout << "  -deb With this option select debug layer to be used (default 0) 0-silent 1-light 2-heavy -deb \"0-2\"" << endl;
	cout << "  -m \"0 or 1\" With this option enable manual check enable(1) or disable(0)(default) -m \"0 or 1\"" << endl;
	cout << endl;
	cout << "Examples:" << endl;
	cout << "  -efile /home/encrText.txt -dfile /home/dencrText.txt -deb 2 -m 1" << endl;
	cout << "  -efile /home/encrText.txt -dfile /home/dencrText.txt -key qwerty" << endl;
	cout << "  -efile /home/encrText.txt -console-output -deb 1" << endl;
	cout << "  -file /home/Text.txt -console-output" << endl;
	cout << "  -selftest 500 -fast 1 -deb 1" << endl;
	cout << "  -console-input-enc -dfile /home/Text.txt -key qwerty -trim 1" << endl;
	cout << "  -console-input-enc console-output -key qwerty" << endl;

#ifdef	__linux__
	system("read");
	//linux code goes here
#elif _WIN32
	system("pause");
	// windows code goes here
#endif
}

int main(int argc, char* argv[])
{
	try
	{

		vector<string> orders;
		debugLayer dLayer = NoDEB;
		bool fast = true;
		bool trim = false;
		string outText;
		// Check the number of parameters
		if (argc < 2) {
			printHelp();
			return 0;
		}
		else
		{
			for (size_t i = 1; i < argc; i++)
			{
				orders.push_back( argv[i]);
			}
		}
		auto deb = find(orders.begin(), orders.end(), "-deb");
		if (deb < orders.end() - 1)
		{
			if (*(deb + 1) == "2")
			{
				dLayer = Heavy;
			}
			else if (*(deb + 1) == "1")
			{
				dLayer = Light;
			}
			else if (*(deb + 1) == "0")
			{
				dLayer = NoDEB;
			}
			else
			{
				printHelp();
				return 0;
			}
		}
		auto f = find(orders.begin(), orders.end(), "-fast");
		if (f < orders.end() - 1)
		{
			if (*(f + 1) == "0")
			{
				fast = false;
			}
			else if (*(f + 1) == "1")
			{
				fast = true;
			}
			else
			{
				printHelp();
				return 0;
			}
		}
		auto t = find(orders.begin(), orders.end(), "-trim");
		if (t < orders.end() - 1)
		{
			if (*(t + 1) == "0")
			{
				fast = false;
			}
			else if (*(t + 1) == "1")
			{
				fast = true;
			}
			else
			{
				printHelp();
				return 0;
			}
		}
		if (orders[0] == "-selftest")
		{
			auto m = find(orders.begin(), orders.end(), "-m");
			bool manualCheck = false;
			if (m < orders.end()-1)
			{
				if (*(m + 1) == "1")
				{
					manualCheck = true;
				}
				else if (*(m + 1) == "0")
				{
					manualCheck = false;
				}
				else
				{
					printHelp();
					return 0;
				}
			}
			uint32_t testTimes = 10000;
			if (regex_match(orders[1].begin(),orders[1].end(),regex("[0-9]+")))
			{
				if (orders[1].size() < 9)
				testTimes = std::stoi(orders[1],nullptr);
			}
		
			string input = "Decryption is performed by going to the row in the table corresponding to the key, finding the position of the ciphertext letter in this row, and then using the column label as the plaintext. For example, in row A (from AYUSH), the ciphertext G appears in column G, which is the first plaintext letter. Next we go to row Y (from AYUSH), locate the ciphertext C which is found in column E, thus E is the second plaintext letter.";

			int wrongSize = 0;
			int good = 0;
			int nogood = 0;
			for (int kl = 1; kl <= testTimes; kl++)
			{
				string startKey = generateKey(kl % MAXKEY + 1);

				string encryptedMsg = encrypt(startKey, input);

				auto origEncryptedMsg = encryptedMsg;
				trimText(encryptedMsg);
				auto allKeysHits = attackOnVegenere(encryptedMsg, dLayer, fast);

				for (auto curKey : allKeysHits)
				{
					auto key = curKey.first;
					string plainText = dencrypt(key, encryptedMsg, origEncryptedMsg);
					if (manualCheck)
					{
						string tmp;
						cout << plainText << endl;
						cout << endl;
						cout << "Is text looks correct? Y/N" << endl;
						cin >> tmp;
						while (!regex_match(tmp.begin(), tmp.begin() + 1, regex("[YyNn]")))
						{
							cout << "Incorrect input, type Y/N" << endl;
							cin >> tmp;
						}

						if (regex_match(tmp.begin(), tmp.begin() + 1, regex("[Yy]")))
						{
							cout << "Job done" << endl;
							cout << "Key is: " << key << endl;
							cout << "Text is: " << endl << plainText << endl << endl;
							break;
						}
						if (curKey == allKeysHits.back())
						{
							cout << "Sorry I failed to decrypt" << '\n';
						}
					}
					else
					{

						if (key.size() != startKey.size())
						{
							if (startKey.find(key) != std::string::npos)
							{
								if (dLayer > 0)
								{
									cout << "Cesar" << good << ' ';
									cout << startKey << " " << key << '\n';
								}
							}
							else
							{
								++wrongSize;
								if (dLayer > 0)
								{
									cout << "good" << good << '\n';
									cout << "notgood" << nogood << '\n';
									cout << "wrongSize" << wrongSize << endl;
								}
							}
						}
						if ((plainText.compare(input) == 0))
						{
							++good;
							if (dLayer > 0)
							{
								cout << "good" << good << '\n';
								cout << "notgood" << nogood << '\n';
								//cout << "wrongSize" << wrongSize << endl;
							}
							break;
						}
						else
						{
							if (curKey == allKeysHits.back())
							{
								nogood++;
								if (dLayer > 0)
								{
									cout << "good" << good << '\n';
									cout << "notgood" << nogood << '\n';
									//cout << "wrongSize" << wrongSize << endl;
								}
							}
						}
					}
				}
			}
			cout << "good" << good << '\n';
			cout << "notgood" << nogood << '\n';
		}
		else if (orders[0] == "-console-input-enc")
		{
			string text;
			if (orders[1].empty())
			{
				cout << "text is empty";
			}
			else
			{
				text = orders[1];
			}

			auto k = find(orders.begin(), orders.end(), "-key");
			string key;
			if (k < orders.end() - 1)
			{
				if ((*(k + 1)).size() < 2 )
				{
					cout << "key to short";
				}
				else if ((*(k + 1)).size() > 12)
				{
					cout << "key to long";
				}
				else
				{
					key = *(k + 1);
					outText = encrypt(key, text);
				}
			}
			else
			{
				cout << "need key to encrypt";
			}
		}
		else if (orders[0] == "-file")
		{
			string line;
			string text;
			string path;
			if (orders[1].empty())
			{
				cout << "text is empty";
			}
			else
			{
				path = orders[1];
			}
			ifstream file(path);
			if (file.is_open())
			{
				while (getline(file, line))
				{
					text.append(line);
					text.append("\n");
				}
				file.close();
				if (text.empty()) cout << "file is empty";
			}
			else
			{
				cout << "Unable to open file";
				return 0;
			}

			auto k = find(orders.begin(), orders.end(), "-key");
			string key;
			if (k < orders.end() - 1)
			{
				if ((*(k + 1)).size() < 2)
				{
					cout << "key to short";
				}
				else if ((*(k + 1)).size() > 12)
				{
					cout << "key to long";
				}
				else
				{
					key = *(k + 1);
					outText = encrypt(key, text);
				}
			}
			else
			{
				cout << "need key to encrypt";
				return 0;
			}
		}
		else if (orders[0] == "-console-input-dec")
		{
			string text;

			string origText;
			if (orders[1].empty())
			{
				cout << "text is empty";
			}
			else
			{
				text = orders[1];
				origText = text;
			}

			auto k = find(orders.begin(), orders.end(), "-key");
			string key;
			if (k < orders.end() - 1)
			{
				if ((*(k + 1)).size() < 2)
				{
					cout << "key to short";
				}
				else if ((*(k + 1)).size() > 12)
				{
					cout << "key to long";
				}
				else 
				{
					key = *(k + 1);
				}
			}
			else
			{
				key = attackOnVegenere(text, dLayer, fast).begin()->first;
			}
			trimText(text);
			outText = dencrypt(key, text, origText);
		}
		else if (orders[0] == "-efile")
		{
			string line;
			string text;
			string origText;
			string path;
			if (orders[1].empty())
			{
				cout << "text is empty";
			}
			else
			{
				path = orders[1];
			}
			ifstream file(path);
			if (file.is_open())
			{
				while (getline(file, line))
				{
					text.append(line);
					text.append("\n");
				}
				file.close();
				if (text.empty()) cout << "file is empty";
				origText = text;
			}
			else
			{
				cout << "Unable to open file";
				return 0;
			}

			auto k = find(orders.begin(), orders.end(), "-key");
			string key;
			if (k < orders.end() - 1)
			{
				if ((*(k + 1)).size() < 2)
				{
					cout << "key to short";
				}
				else if ((*(k + 1)).size() > 12)
				{
					cout << "key to long";
				}
				else
				{
					key = *(k + 1);
				}
			}
			else
			{
				key = attackOnVegenere(text, dLayer, fast).begin()->first;
			}
			trimText(text);
			outText = dencrypt(key, text, origText);
		}
		else
		{
			printHelp();
			return 0;
		}

		if (trim) trimText(outText);

		auto out = find(orders.begin(), orders.end(), "-console-output");
		if (out < orders.end())
		{
			cout << outText;
			return 0;
		}
		out = find(orders.begin(), orders.end(), "-dfile");
		if (out < orders.end() - 1)
		{
			string line;
			string path;
			if ((*(out + 1)).empty())
			{
				cout << "dir is empty";
			}
			else
			{
				path = *(out + 1);
			}
			ofstream file(path);
			if (file.is_open())
			{

				file << outText;
				file << "\n";
				file << "\n";
	
				file.close();
			}
			else
			{
				cout << "Unable to open file";
				return 0;
			}
		}
		else
		{
			printHelp();
			return 0;
		}

		//cin >> a;
		return 0;
	}
	catch (const char* a)
	{
		cout << a;
		return 1;
	}
}

