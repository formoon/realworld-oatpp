#include "deunicode.hpp"

using namespace std;

#define POINTERLENGTH 389253
string MAPPING;
unsigned char BUF[POINTERLENGTH];

std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> CVT;

struct Ptr {
    uint16_t offset;
    uint8_t len;
} __attribute__((packed));

struct Ptr *charPtr = (struct Ptr *)BUF;

std::unordered_map<std::string, std::string> charMap {
    // latin
    {"À", "A"}, {"Á", "A"}, {"Â", "A"}, {"Ã", "A"}, {"Ä", "A"}, {"Å", "A"}, {"Æ", "AE"}, {
    "Ç", "C"}, {"È", "E"}, {"É", "E"}, {"Ê", "E"}, {"Ë", "E"}, {"Ì", "I"}, {"Í", "I"}, {
    "Î", "I"}, {"Ï", "I"}, {"Ð", "D"}, {"Ñ", "N"}, {"Ò", "O"}, {"Ó", "O"}, {"Ô", "O"}, {
    "Õ", "O"}, {"Ö", "O"}, {"Ő", "O"}, {"Ø", "O"}, {"Ù", "U"}, {"Ú", "U"}, {"Û", "U"}, {
    "Ü", "U"}, {"Ű", "U"}, {"Ý", "Y"}, {"Þ", "TH"}, {"ß", "ss"}, {"à", "a"}, {"á", "a"}, {
    "â", "a"}, {"ã", "a"}, {"ä", "a"}, {"å", "a"}, {"æ", "ae"}, {"ç", "c"}, {"è", "e"}, {
    "é", "e"}, {"ê", "e"}, {"ë", "e"}, {"ì", "i"}, {"í", "i"}, {"î", "i"}, {"ï", "i"}, {
    "ð", "d"}, {"ñ", "n"}, {"ò", "o"}, {"ó", "o"}, {"ô", "o"}, {"õ", "o"}, {"ö", "o"}, {
    "ő", "o"}, {"ø", "o"}, {"ù", "u"}, {"ú", "u"}, {"û", "u"}, {"ü", "u"}, {"ű", "u"}, {
    "ý", "y"}, {"þ", "th"}, {"ÿ", "y"}, {"ẞ", "SS"},
    // greek
    {"α", "a"}, {"β", "b"}, {"γ", "g"}, {"δ", "d"}, {"ε", "e"}, {"ζ", "z"}, {"η", "h"}, {"θ", "8"}, {
    "ι", "i"}, {"κ", "k"}, {"λ", "l"}, {"μ", "m"}, {"ν", "n"}, {"ξ", "3"}, {"ο", "o"}, {"π", "p"}, {
    "ρ", "r"}, {"σ", "s"}, {"τ", "t"}, {"υ", "y"}, {"φ", "f"}, {"χ", "x"}, {"ψ", "ps"}, {"ω", "w"}, {
    "ά", "a"}, {"έ", "e"}, {"ί", "i"}, {"ό", "o"}, {"ύ", "y"}, {"ή", "h"}, {"ώ", "w"}, {"ς", "s"}, {
    "ϊ", "i"}, {"ΰ", "y"}, {"ϋ", "y"}, {"ΐ", "i"}, {
    "Α", "A"}, {"Β", "B"}, {"Γ", "G"}, {"Δ", "D"}, {"Ε", "E"}, {"Ζ", "Z"}, {"Η", "H"}, {"Θ", "8"}, {
    "Ι", "I"}, {"Κ", "K"}, {"Λ", "L"}, {"Μ", "M"}, {"Ν", "N"}, {"Ξ", "3"}, {"Ο", "O"}, {"Π", "P"}, {
    "Ρ", "R"}, {"Σ", "S"}, {"Τ", "T"}, {"Υ", "Y"}, {"Φ", "F"}, {"Χ", "X"}, {"Ψ", "PS"}, {"Ω", "W"}, {
    "Ά", "A"}, {"Έ", "E"}, {"Ί", "I"}, {"Ό", "O"}, {"Ύ", "Y"}, {"Ή", "H"}, {"Ώ", "W"}, {"Ϊ", "I"}, {
    "Ϋ", "Y"},
    // turkish
    {"ş", "s"}, {"Ş", "S"}, {"ı", "i"}, {"İ", "I"}, {"ç", "c"}, {"Ç", "C"}, {"ü", "u"}, {"Ü", "U"}, {
    "ö", "o"}, {"Ö", "O"}, {"ğ", "g"}, {"Ğ", "G"},
    // russian
    {"а", "a"}, {"б", "b"}, {"в", "v"}, {"г", "g"}, {"д", "d"}, {"е", "e"}, {"ё", "yo"}, {"ж", "zh"}, {
    "з", "z"}, {"и", "i"}, {"й", "j"}, {"к", "k"}, {"л", "l"}, {"м", "m"}, {"н", "n"}, {"о", "o"}, {
    "п", "p"}, {"р", "r"}, {"с", "s"}, {"т", "t"}, {"у", "u"}, {"ф", "f"}, {"х", "h"}, {"ц", "c"}, {
    "ч", "ch"}, {"ш", "sh"}, {"щ", "sh"}, {"ъ", "u"}, {"ы", "y"}, {"ь", ""}, {"э", "e"}, {"ю", "yu"}, {
    "я", "ya"}, {
    "А", "A"}, {"Б", "B"}, {"В", "V"}, {"Г", "G"}, {"Д", "D"}, {"Е", "E"}, {"Ё", "Yo"}, {"Ж", "Zh"}, {
    "З", "Z"}, {"И", "I"}, {"Й", "J"}, {"К", "K"}, {"Л", "L"}, {"М", "M"}, {"Н", "N"}, {"О", "O"}, {
    "П", "P"}, {"Р", "R"}, {"С", "S"}, {"Т", "T"}, {"У", "U"}, {"Ф", "F"}, {"Х", "H"}, {"Ц", "C"}, {
    "Ч", "Ch"}, {"Ш", "Sh"}, {"Щ", "Sh"}, {"Ъ", "U"}, {"Ы", "Y"}, {"Ь", ""}, {"Э", "E"}, {"Ю", "Yu"}, {
    "Я", "Ya"},
    // ukranian
    {"Є", "Ye"}, {"І", "I"}, {"Ї", "Yi"}, {"Ґ", "G"}, {"є", "ye"}, {"і", "i"}, {"ї", "yi"}, {"ґ", "g"},
    // czech
    {"č", "c"}, {"ď", "d"}, {"ě", "e"}, {"ň", "n"}, {"ř", "r"}, {"š", "s"}, {"ť", "t"}, {"ů", "u"},
    {"ž", "z"}, {"Č", "C"}, {"Ď", "D"}, {"Ě", "E"}, {"Ň", "N"}, {"Ř", "R"}, {"Š", "S"}, {"Ť", "T"},
    {"Ů", "U"}, {"Ž", "Z"},
    // polish
    {"ą", "a"}, {"ć", "c"}, {"ę", "e"}, {"ł", "l"}, {"ń", "n"}, {"ó", "o"}, {"ś", "s"}, {"ź", "z"},
    {"ż", "z"}, {"Ą", "A"}, {"Ć", "C"}, {"Ę", "e"}, {"Ł", "L"}, {"Ń", "N"}, {"Ś", "S"},
    {"Ź", "Z"}, {"Ż", "Z"},
    // latvian
    {"ā", "a"}, {"č", "c"}, {"ē", "e"}, {"ģ", "g"}, {"ī", "i"}, {"ķ", "k"}, {"ļ", "l"}, {"ņ", "n"},
    {"š", "s"}, {"ū", "u"}, {"ž", "z"}, {"Ā", "A"}, {"Č", "C"}, {"Ē", "E"}, {"Ģ", "G"}, {"Ī", "i"},
    {"Ķ", "k"}, {"Ļ", "L"}, {"Ņ", "N"}, {"Š", "S"}, {"Ū", "u"}, {"Ž", "Z"},
    // currency
    {"€", "euro"}, {"₢", "cruzeiro"}, {"₣", "french franc"}, {"£", "pound"},
    {"₤", "lira"}, {"₥", "mill"}, {"₦", "naira"}, {"₧", "peseta"}, {"₨", "rupee"},
    {"₩", "won"}, {"₪", "new shequel"}, {"₫", "dong"}, {"₭", "kip"}, {"₮", "tugrik"},
    {"₯", "drachma"}, {"₰", "penny"}, {"₱", "peso"}, {"₲", "guarani"}, {"₳", "austral"},
    {"₴", "hryvnia"}, {"₵", "cedi"}, {"¢", "cent"}, {"¥", "yen"}, {"元", "yuan"},
    {"円", "yen"}, {"﷼", "rial"}, {"₠", "ecu"}, {"¤", "currency"}, {"฿", "baht"}, {"$", "dollar"},
    // symbols
    {"©", "(c)"}, {"œ", "oe"}, {"Œ", "OE"}, {"∑", "sum"}, {"®", "(r)"}, {"†", "+"},
    {"“", "\""}, {"∂", "d"}, {"ƒ", "f"}, {"™", "tm"},
    {"℠", "sm"}, {"…", "..."}, {"˚", "o"}, {"º", "o"}, {"ª", "a"}, {"•", "*"},
    {"∆", "delta"}, {"∞", "infinity"}, {"♥", "love"}, {"&", "and"}, {"|", "or"},
    {"<", "less"}, {">", "greater"}
};

template<typename tStringType, typename tTraits = typename tStringType::traits_type>
void print_code_unit_sequence(tStringType str)
{
    using char_type = typename tTraits::char_type;
    static_assert(std::is_same<char_type, char>::value || std::is_same<char_type, char16_t>::value || std::is_same<char_type, char32_t>::value, "error");
    using unsigned_char_type = typename std::make_unsigned<char_type>::type;
    using unsigned_int_type = typename std::make_unsigned<typename tTraits::int_type>::type;
    int w = std::is_same<char, char_type>::value ? 2 : std::is_same<char16_t, char_type>::value ? 4 : 8;
    for(auto c : str) {
        auto value = static_cast<unsigned_int_type>(static_cast<unsigned_char_type>(c));
        std::cout << "0x" << std::hex << std::uppercase << std::setw(w) << std::setfill('0') << value << ' ';
    }
}

string readMapping(){
    std::ifstream t("unicode/mapping.txt");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}
void readPointer(unsigned char *p){
    std::ifstream infile("unicode/pointers.bin");
    infile.read((char *)p, POINTERLENGTH);
}
int LE2BE(int c){
    return ((c&0xff) << 8) + ((c&0xff00)>>8);
}

void dumpPointer(){
    int len=POINTERLENGTH/3;
    for(int i=0;i<0x300;i++){
        cout << hex << charPtr[i].offset << " : " << hex << (int)charPtr[i].len << endl;
    };
}

void unicode_demo(){
    string a8=u8"北京abc";
    u16string a16=u"北京abc";
    u32string a32=U"北京abc";

    cout << "u8: " << a8  << endl;
    print_code_unit_sequence(a8);
    cout << " " << a8.length() << endl;
    cout << "u16:" << endl;
    print_code_unit_sequence(a16);
    cout << " " << a16.length() << endl;
    cout << "u32:" << endl;
    print_code_unit_sequence(a32);
    cout << " " << a32.length() << endl;

    cout << "convert u8 to u16:" << endl;
    std::u16string a16FromA8 = CVT.from_bytes(a8); // utf-8 to utf-16
    print_code_unit_sequence(a16FromA8);
    cout <<  " " << a16FromA8.length() << endl;
}

string slugify(string sin){
    /*
    int fi;
	for(auto kv : charMap){
		// check if key is in string
        fi=sin.find(kv.first);
		if( fi != std::string::npos){
			// replace key with value
			sin.replace(fi, kv.first.length(), kv.second);
		}
	}*/
    auto s=CVT.from_bytes(sin);
    string rs=u8"";
    for(auto c:s){
        if ((uint16_t)c < 0xff){
            rs += ((char)c);
            continue;
        }
        int c1=(uint16_t)c;
        if (c1 >= POINTERLENGTH/3){
            // rs += CVT.to_bytes(c);
            // rs.push_back(c);
            continue;
        }
        int offset=(int)charPtr[c1].offset;
        int len=(int)charPtr[c1].len;
        if(offset > MAPPING.length()-len){
            // rs += CVT.to_bytes(c);
            continue;
        }
        for(auto cs:MAPPING.substr(offset,len)){
            rs += cs;
        }
    };

	std::regex e1("[^\\w\\s$*_+~.()\'\"-]");
	rs = std::regex_replace(rs, e1, "");

	std::regex e2("^\\s+|\\s+$");
	rs = std::regex_replace(rs, e2, "");

	std::regex e3("[-\\s]+");
	rs = std::regex_replace(rs, e3, "-");

	std::regex e4("#-$");
	rs = std::regex_replace(rs, e4, "");

    return rs;
};

void slugify_init(){
    MAPPING = readMapping();
    readPointer(BUF);

    // dumpPointer();
    // unicode_demo();

    // string testStr="北京abc❤️♥";
    // auto s=slugify(testStr);
    // // auto s1=CVT.to_bytes(u"北京abc❤️");
    // cout << s << " " << s.length()<< endl;
    // cout << testStr << " " << testStr.length()<< endl;
}

