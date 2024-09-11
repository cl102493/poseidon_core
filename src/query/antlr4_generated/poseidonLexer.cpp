
// Generated from src/query/parser/poseidon.g4 by ANTLR 4.13.1


#include "poseidonLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct PoseidonLexerStaticData final {
  PoseidonLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  PoseidonLexerStaticData(const PoseidonLexerStaticData&) = delete;
  PoseidonLexerStaticData(PoseidonLexerStaticData&&) = delete;
  PoseidonLexerStaticData& operator=(const PoseidonLexerStaticData&) = delete;
  PoseidonLexerStaticData& operator=(PoseidonLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag poseidonlexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
PoseidonLexerStaticData *poseidonlexerLexerStaticData = nullptr;

void poseidonlexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (poseidonlexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(poseidonlexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<PoseidonLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "Filter_", 
      "Nodescan_", "Indexscan_", "Match_", "Project_", "Limit_", "CrossJoin_", 
      "HashJoin_", "LeftOuterJoin_", "Expand_", "ForeachRelationship_", 
      "Aggregate_", "GroupBy_", "Sort_", "Distinct_", "Create_", "Union_", 
      "RemoveNode_", "RemoveRelationship_", "DetachNode_", "Algorithm_", 
      "IntType_", "Uint64Type_", "DoubleType_", "StringType_", "DateType_", 
      "ResultType_", "Count_", "Sum_", "Avg_", "Min_", "Max_", "UDF_", "BUILTIN_", 
      "InExpandDir_", "OutExpandDir_", "TupleMode_", "ResultSetMode_", "FromDir_", 
      "ToDir_", "AllDir_", "DescOrder_", "AscOrder_", "INTEGER", "FLOAT", 
      "Identifier_", "Var", "NAME_", "STRING_", "COLON_", "DOUBLE_COLON", 
      "COMMA_", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "OR", "AND", 
      "EQUALS", "NOTEQUALS", "LT", "LTEQ", "GT", "GTEQ", "PLUS_", "MULT", 
      "DIV", "MOD", "NOT", "WHITESPACE"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'.'", "'..'", "'<-'", "'->'", "'-'", "'{'", "'}'", "'Filter'", 
      "'NodeScan'", "'IndexScan'", "'Match'", "'Project'", "'Limit'", "'CrossJoin'", 
      "'HashJoin'", "'LeftOuterJoin'", "'Expand'", "'ForeachRelationship'", 
      "'Aggregate'", "'GroupBy'", "'Sort'", "'Distinct'", "'Create'", "'Union'", 
      "'RemoveNode'", "'RemoveRelationship'", "'DetachNode'", "'Algorithm'", 
      "'int'", "'uint64'", "'double'", "'string'", "'datetime'", "'qresult'", 
      "'count'", "'sum'", "'avg'", "'min'", "'max'", "'udf'", "'pb'", "'IN'", 
      "'OUT'", "'TUPLE'", "'SET'", "'FROM'", "'TO'", "'ALL'", "'DESC'", 
      "'ASC'", "", "", "", "", "", "", "':'", "'::'", "','", "'('", "')'", 
      "'['", "']'", "", "", "", "", "'<'", "'<='", "'>'", "'>='", "'+'", 
      "'*'", "'/'", "'%'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "Filter_", "Nodescan_", "Indexscan_", 
      "Match_", "Project_", "Limit_", "CrossJoin_", "HashJoin_", "LeftOuterJoin_", 
      "Expand_", "ForeachRelationship_", "Aggregate_", "GroupBy_", "Sort_", 
      "Distinct_", "Create_", "Union_", "RemoveNode_", "RemoveRelationship_", 
      "DetachNode_", "Algorithm_", "IntType_", "Uint64Type_", "DoubleType_", 
      "StringType_", "DateType_", "ResultType_", "Count_", "Sum_", "Avg_", 
      "Min_", "Max_", "UDF_", "BUILTIN_", "InExpandDir_", "OutExpandDir_", 
      "TupleMode_", "ResultSetMode_", "FromDir_", "ToDir_", "AllDir_", "DescOrder_", 
      "AscOrder_", "INTEGER", "FLOAT", "Identifier_", "Var", "NAME_", "STRING_", 
      "COLON_", "DOUBLE_COLON", "COMMA_", "LPAREN", "RPAREN", "LBRACKET", 
      "RBRACKET", "OR", "AND", "EQUALS", "NOTEQUALS", "LT", "LTEQ", "GT", 
      "GTEQ", "PLUS_", "MULT", "DIV", "MOD", "NOT", "WHITESPACE"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,77,594,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,
  	7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,
  	7,42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,
  	7,49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,
  	7,56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,
  	7,63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,
  	7,70,2,71,7,71,2,72,7,72,2,73,7,73,2,74,7,74,2,75,7,75,2,76,7,76,1,0,
  	1,0,1,1,1,1,1,1,1,2,1,2,1,2,1,3,1,3,1,3,1,4,1,4,1,5,1,5,1,6,1,6,1,7,1,
  	7,1,7,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,9,1,9,1,9,
  	1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,10,1,10,1,10,1,10,1,10,1,10,1,11,1,11,1,
  	11,1,11,1,11,1,11,1,11,1,11,1,12,1,12,1,12,1,12,1,12,1,12,1,13,1,13,1,
  	13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,14,1,14,1,14,1,14,1,14,1,14,1,
  	14,1,14,1,14,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,
  	15,1,15,1,15,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,17,1,17,1,17,1,17,1,
  	17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,
  	17,1,17,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,19,1,19,1,
  	19,1,19,1,19,1,19,1,19,1,19,1,20,1,20,1,20,1,20,1,20,1,21,1,21,1,21,1,
  	21,1,21,1,21,1,21,1,21,1,21,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,23,1,
  	23,1,23,1,23,1,23,1,23,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,
  	24,1,24,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,
  	25,1,25,1,25,1,25,1,25,1,25,1,25,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,
  	26,1,26,1,26,1,26,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,
  	28,1,28,1,28,1,28,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,30,1,30,1,30,1,
  	30,1,30,1,30,1,30,1,31,1,31,1,31,1,31,1,31,1,31,1,31,1,32,1,32,1,32,1,
  	32,1,32,1,32,1,32,1,32,1,32,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,
  	34,1,34,1,34,1,34,1,34,1,34,1,35,1,35,1,35,1,35,1,36,1,36,1,36,1,36,1,
  	37,1,37,1,37,1,37,1,38,1,38,1,38,1,38,1,39,1,39,1,39,1,39,1,40,1,40,1,
  	40,1,41,1,41,1,41,1,42,1,42,1,42,1,42,1,43,1,43,1,43,1,43,1,43,1,43,1,
  	44,1,44,1,44,1,44,1,45,1,45,1,45,1,45,1,45,1,46,1,46,1,46,1,47,1,47,1,
  	47,1,47,1,48,1,48,1,48,1,48,1,48,1,49,1,49,1,49,1,49,1,50,4,50,485,8,
  	50,11,50,12,50,486,1,51,3,51,490,8,51,1,51,4,51,493,8,51,11,51,12,51,
  	494,1,51,1,51,4,51,499,8,51,11,51,12,51,500,1,52,1,52,1,53,1,53,4,53,
  	507,8,53,11,53,12,53,508,1,54,4,54,512,8,54,11,54,12,54,513,1,55,1,55,
  	5,55,518,8,55,10,55,12,55,521,9,55,1,55,1,55,1,56,1,56,1,57,1,57,1,57,
  	1,58,1,58,1,59,1,59,1,60,1,60,1,61,1,61,1,62,1,62,1,63,1,63,1,63,1,63,
  	3,63,544,8,63,1,64,1,64,1,64,1,64,1,64,3,64,551,8,64,1,65,1,65,1,65,3,
  	65,556,8,65,1,66,1,66,1,66,1,66,3,66,562,8,66,1,67,1,67,1,68,1,68,1,68,
  	1,69,1,69,1,70,1,70,1,70,1,71,1,71,1,72,1,72,1,73,1,73,1,74,1,74,1,75,
  	1,75,1,75,1,75,3,75,586,8,75,1,76,4,76,589,8,76,11,76,12,76,590,1,76,
  	1,76,1,519,0,77,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,
  	12,25,13,27,14,29,15,31,16,33,17,35,18,37,19,39,20,41,21,43,22,45,23,
  	47,24,49,25,51,26,53,27,55,28,57,29,59,30,61,31,63,32,65,33,67,34,69,
  	35,71,36,73,37,75,38,77,39,79,40,81,41,83,42,85,43,87,44,89,45,91,46,
  	93,47,95,48,97,49,99,50,101,51,103,52,105,53,107,54,109,55,111,56,113,
  	57,115,58,117,59,119,60,121,61,123,62,125,63,127,64,129,65,131,66,133,
  	67,135,68,137,69,139,70,141,71,143,72,145,73,147,74,149,75,151,76,153,
  	77,1,0,3,1,0,48,57,5,0,45,45,48,57,65,90,95,95,97,122,3,0,9,10,13,13,
  	32,32,606,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,
  	0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,
  	1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,
  	0,0,0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,0,0,0,0,41,1,0,0,0,
  	0,43,1,0,0,0,0,45,1,0,0,0,0,47,1,0,0,0,0,49,1,0,0,0,0,51,1,0,0,0,0,53,
  	1,0,0,0,0,55,1,0,0,0,0,57,1,0,0,0,0,59,1,0,0,0,0,61,1,0,0,0,0,63,1,0,
  	0,0,0,65,1,0,0,0,0,67,1,0,0,0,0,69,1,0,0,0,0,71,1,0,0,0,0,73,1,0,0,0,
  	0,75,1,0,0,0,0,77,1,0,0,0,0,79,1,0,0,0,0,81,1,0,0,0,0,83,1,0,0,0,0,85,
  	1,0,0,0,0,87,1,0,0,0,0,89,1,0,0,0,0,91,1,0,0,0,0,93,1,0,0,0,0,95,1,0,
  	0,0,0,97,1,0,0,0,0,99,1,0,0,0,0,101,1,0,0,0,0,103,1,0,0,0,0,105,1,0,0,
  	0,0,107,1,0,0,0,0,109,1,0,0,0,0,111,1,0,0,0,0,113,1,0,0,0,0,115,1,0,0,
  	0,0,117,1,0,0,0,0,119,1,0,0,0,0,121,1,0,0,0,0,123,1,0,0,0,0,125,1,0,0,
  	0,0,127,1,0,0,0,0,129,1,0,0,0,0,131,1,0,0,0,0,133,1,0,0,0,0,135,1,0,0,
  	0,0,137,1,0,0,0,0,139,1,0,0,0,0,141,1,0,0,0,0,143,1,0,0,0,0,145,1,0,0,
  	0,0,147,1,0,0,0,0,149,1,0,0,0,0,151,1,0,0,0,0,153,1,0,0,0,1,155,1,0,0,
  	0,3,157,1,0,0,0,5,160,1,0,0,0,7,163,1,0,0,0,9,166,1,0,0,0,11,168,1,0,
  	0,0,13,170,1,0,0,0,15,172,1,0,0,0,17,179,1,0,0,0,19,188,1,0,0,0,21,198,
  	1,0,0,0,23,204,1,0,0,0,25,212,1,0,0,0,27,218,1,0,0,0,29,228,1,0,0,0,31,
  	237,1,0,0,0,33,251,1,0,0,0,35,258,1,0,0,0,37,278,1,0,0,0,39,288,1,0,0,
  	0,41,296,1,0,0,0,43,301,1,0,0,0,45,310,1,0,0,0,47,317,1,0,0,0,49,323,
  	1,0,0,0,51,334,1,0,0,0,53,353,1,0,0,0,55,364,1,0,0,0,57,374,1,0,0,0,59,
  	378,1,0,0,0,61,385,1,0,0,0,63,392,1,0,0,0,65,399,1,0,0,0,67,408,1,0,0,
  	0,69,416,1,0,0,0,71,422,1,0,0,0,73,426,1,0,0,0,75,430,1,0,0,0,77,434,
  	1,0,0,0,79,438,1,0,0,0,81,442,1,0,0,0,83,445,1,0,0,0,85,448,1,0,0,0,87,
  	452,1,0,0,0,89,458,1,0,0,0,91,462,1,0,0,0,93,467,1,0,0,0,95,470,1,0,0,
  	0,97,474,1,0,0,0,99,479,1,0,0,0,101,484,1,0,0,0,103,489,1,0,0,0,105,502,
  	1,0,0,0,107,504,1,0,0,0,109,511,1,0,0,0,111,515,1,0,0,0,113,524,1,0,0,
  	0,115,526,1,0,0,0,117,529,1,0,0,0,119,531,1,0,0,0,121,533,1,0,0,0,123,
  	535,1,0,0,0,125,537,1,0,0,0,127,543,1,0,0,0,129,550,1,0,0,0,131,555,1,
  	0,0,0,133,561,1,0,0,0,135,563,1,0,0,0,137,565,1,0,0,0,139,568,1,0,0,0,
  	141,570,1,0,0,0,143,573,1,0,0,0,145,575,1,0,0,0,147,577,1,0,0,0,149,579,
  	1,0,0,0,151,585,1,0,0,0,153,588,1,0,0,0,155,156,5,46,0,0,156,2,1,0,0,
  	0,157,158,5,46,0,0,158,159,5,46,0,0,159,4,1,0,0,0,160,161,5,60,0,0,161,
  	162,5,45,0,0,162,6,1,0,0,0,163,164,5,45,0,0,164,165,5,62,0,0,165,8,1,
  	0,0,0,166,167,5,45,0,0,167,10,1,0,0,0,168,169,5,123,0,0,169,12,1,0,0,
  	0,170,171,5,125,0,0,171,14,1,0,0,0,172,173,5,70,0,0,173,174,5,105,0,0,
  	174,175,5,108,0,0,175,176,5,116,0,0,176,177,5,101,0,0,177,178,5,114,0,
  	0,178,16,1,0,0,0,179,180,5,78,0,0,180,181,5,111,0,0,181,182,5,100,0,0,
  	182,183,5,101,0,0,183,184,5,83,0,0,184,185,5,99,0,0,185,186,5,97,0,0,
  	186,187,5,110,0,0,187,18,1,0,0,0,188,189,5,73,0,0,189,190,5,110,0,0,190,
  	191,5,100,0,0,191,192,5,101,0,0,192,193,5,120,0,0,193,194,5,83,0,0,194,
  	195,5,99,0,0,195,196,5,97,0,0,196,197,5,110,0,0,197,20,1,0,0,0,198,199,
  	5,77,0,0,199,200,5,97,0,0,200,201,5,116,0,0,201,202,5,99,0,0,202,203,
  	5,104,0,0,203,22,1,0,0,0,204,205,5,80,0,0,205,206,5,114,0,0,206,207,5,
  	111,0,0,207,208,5,106,0,0,208,209,5,101,0,0,209,210,5,99,0,0,210,211,
  	5,116,0,0,211,24,1,0,0,0,212,213,5,76,0,0,213,214,5,105,0,0,214,215,5,
  	109,0,0,215,216,5,105,0,0,216,217,5,116,0,0,217,26,1,0,0,0,218,219,5,
  	67,0,0,219,220,5,114,0,0,220,221,5,111,0,0,221,222,5,115,0,0,222,223,
  	5,115,0,0,223,224,5,74,0,0,224,225,5,111,0,0,225,226,5,105,0,0,226,227,
  	5,110,0,0,227,28,1,0,0,0,228,229,5,72,0,0,229,230,5,97,0,0,230,231,5,
  	115,0,0,231,232,5,104,0,0,232,233,5,74,0,0,233,234,5,111,0,0,234,235,
  	5,105,0,0,235,236,5,110,0,0,236,30,1,0,0,0,237,238,5,76,0,0,238,239,5,
  	101,0,0,239,240,5,102,0,0,240,241,5,116,0,0,241,242,5,79,0,0,242,243,
  	5,117,0,0,243,244,5,116,0,0,244,245,5,101,0,0,245,246,5,114,0,0,246,247,
  	5,74,0,0,247,248,5,111,0,0,248,249,5,105,0,0,249,250,5,110,0,0,250,32,
  	1,0,0,0,251,252,5,69,0,0,252,253,5,120,0,0,253,254,5,112,0,0,254,255,
  	5,97,0,0,255,256,5,110,0,0,256,257,5,100,0,0,257,34,1,0,0,0,258,259,5,
  	70,0,0,259,260,5,111,0,0,260,261,5,114,0,0,261,262,5,101,0,0,262,263,
  	5,97,0,0,263,264,5,99,0,0,264,265,5,104,0,0,265,266,5,82,0,0,266,267,
  	5,101,0,0,267,268,5,108,0,0,268,269,5,97,0,0,269,270,5,116,0,0,270,271,
  	5,105,0,0,271,272,5,111,0,0,272,273,5,110,0,0,273,274,5,115,0,0,274,275,
  	5,104,0,0,275,276,5,105,0,0,276,277,5,112,0,0,277,36,1,0,0,0,278,279,
  	5,65,0,0,279,280,5,103,0,0,280,281,5,103,0,0,281,282,5,114,0,0,282,283,
  	5,101,0,0,283,284,5,103,0,0,284,285,5,97,0,0,285,286,5,116,0,0,286,287,
  	5,101,0,0,287,38,1,0,0,0,288,289,5,71,0,0,289,290,5,114,0,0,290,291,5,
  	111,0,0,291,292,5,117,0,0,292,293,5,112,0,0,293,294,5,66,0,0,294,295,
  	5,121,0,0,295,40,1,0,0,0,296,297,5,83,0,0,297,298,5,111,0,0,298,299,5,
  	114,0,0,299,300,5,116,0,0,300,42,1,0,0,0,301,302,5,68,0,0,302,303,5,105,
  	0,0,303,304,5,115,0,0,304,305,5,116,0,0,305,306,5,105,0,0,306,307,5,110,
  	0,0,307,308,5,99,0,0,308,309,5,116,0,0,309,44,1,0,0,0,310,311,5,67,0,
  	0,311,312,5,114,0,0,312,313,5,101,0,0,313,314,5,97,0,0,314,315,5,116,
  	0,0,315,316,5,101,0,0,316,46,1,0,0,0,317,318,5,85,0,0,318,319,5,110,0,
  	0,319,320,5,105,0,0,320,321,5,111,0,0,321,322,5,110,0,0,322,48,1,0,0,
  	0,323,324,5,82,0,0,324,325,5,101,0,0,325,326,5,109,0,0,326,327,5,111,
  	0,0,327,328,5,118,0,0,328,329,5,101,0,0,329,330,5,78,0,0,330,331,5,111,
  	0,0,331,332,5,100,0,0,332,333,5,101,0,0,333,50,1,0,0,0,334,335,5,82,0,
  	0,335,336,5,101,0,0,336,337,5,109,0,0,337,338,5,111,0,0,338,339,5,118,
  	0,0,339,340,5,101,0,0,340,341,5,82,0,0,341,342,5,101,0,0,342,343,5,108,
  	0,0,343,344,5,97,0,0,344,345,5,116,0,0,345,346,5,105,0,0,346,347,5,111,
  	0,0,347,348,5,110,0,0,348,349,5,115,0,0,349,350,5,104,0,0,350,351,5,105,
  	0,0,351,352,5,112,0,0,352,52,1,0,0,0,353,354,5,68,0,0,354,355,5,101,0,
  	0,355,356,5,116,0,0,356,357,5,97,0,0,357,358,5,99,0,0,358,359,5,104,0,
  	0,359,360,5,78,0,0,360,361,5,111,0,0,361,362,5,100,0,0,362,363,5,101,
  	0,0,363,54,1,0,0,0,364,365,5,65,0,0,365,366,5,108,0,0,366,367,5,103,0,
  	0,367,368,5,111,0,0,368,369,5,114,0,0,369,370,5,105,0,0,370,371,5,116,
  	0,0,371,372,5,104,0,0,372,373,5,109,0,0,373,56,1,0,0,0,374,375,5,105,
  	0,0,375,376,5,110,0,0,376,377,5,116,0,0,377,58,1,0,0,0,378,379,5,117,
  	0,0,379,380,5,105,0,0,380,381,5,110,0,0,381,382,5,116,0,0,382,383,5,54,
  	0,0,383,384,5,52,0,0,384,60,1,0,0,0,385,386,5,100,0,0,386,387,5,111,0,
  	0,387,388,5,117,0,0,388,389,5,98,0,0,389,390,5,108,0,0,390,391,5,101,
  	0,0,391,62,1,0,0,0,392,393,5,115,0,0,393,394,5,116,0,0,394,395,5,114,
  	0,0,395,396,5,105,0,0,396,397,5,110,0,0,397,398,5,103,0,0,398,64,1,0,
  	0,0,399,400,5,100,0,0,400,401,5,97,0,0,401,402,5,116,0,0,402,403,5,101,
  	0,0,403,404,5,116,0,0,404,405,5,105,0,0,405,406,5,109,0,0,406,407,5,101,
  	0,0,407,66,1,0,0,0,408,409,5,113,0,0,409,410,5,114,0,0,410,411,5,101,
  	0,0,411,412,5,115,0,0,412,413,5,117,0,0,413,414,5,108,0,0,414,415,5,116,
  	0,0,415,68,1,0,0,0,416,417,5,99,0,0,417,418,5,111,0,0,418,419,5,117,0,
  	0,419,420,5,110,0,0,420,421,5,116,0,0,421,70,1,0,0,0,422,423,5,115,0,
  	0,423,424,5,117,0,0,424,425,5,109,0,0,425,72,1,0,0,0,426,427,5,97,0,0,
  	427,428,5,118,0,0,428,429,5,103,0,0,429,74,1,0,0,0,430,431,5,109,0,0,
  	431,432,5,105,0,0,432,433,5,110,0,0,433,76,1,0,0,0,434,435,5,109,0,0,
  	435,436,5,97,0,0,436,437,5,120,0,0,437,78,1,0,0,0,438,439,5,117,0,0,439,
  	440,5,100,0,0,440,441,5,102,0,0,441,80,1,0,0,0,442,443,5,112,0,0,443,
  	444,5,98,0,0,444,82,1,0,0,0,445,446,5,73,0,0,446,447,5,78,0,0,447,84,
  	1,0,0,0,448,449,5,79,0,0,449,450,5,85,0,0,450,451,5,84,0,0,451,86,1,0,
  	0,0,452,453,5,84,0,0,453,454,5,85,0,0,454,455,5,80,0,0,455,456,5,76,0,
  	0,456,457,5,69,0,0,457,88,1,0,0,0,458,459,5,83,0,0,459,460,5,69,0,0,460,
  	461,5,84,0,0,461,90,1,0,0,0,462,463,5,70,0,0,463,464,5,82,0,0,464,465,
  	5,79,0,0,465,466,5,77,0,0,466,92,1,0,0,0,467,468,5,84,0,0,468,469,5,79,
  	0,0,469,94,1,0,0,0,470,471,5,65,0,0,471,472,5,76,0,0,472,473,5,76,0,0,
  	473,96,1,0,0,0,474,475,5,68,0,0,475,476,5,69,0,0,476,477,5,83,0,0,477,
  	478,5,67,0,0,478,98,1,0,0,0,479,480,5,65,0,0,480,481,5,83,0,0,481,482,
  	5,67,0,0,482,100,1,0,0,0,483,485,7,0,0,0,484,483,1,0,0,0,485,486,1,0,
  	0,0,486,484,1,0,0,0,486,487,1,0,0,0,487,102,1,0,0,0,488,490,5,45,0,0,
  	489,488,1,0,0,0,489,490,1,0,0,0,490,492,1,0,0,0,491,493,2,48,57,0,492,
  	491,1,0,0,0,493,494,1,0,0,0,494,492,1,0,0,0,494,495,1,0,0,0,495,496,1,
  	0,0,0,496,498,5,46,0,0,497,499,2,48,57,0,498,497,1,0,0,0,499,500,1,0,
  	0,0,500,498,1,0,0,0,500,501,1,0,0,0,501,104,1,0,0,0,502,503,3,109,54,
  	0,503,106,1,0,0,0,504,506,5,36,0,0,505,507,7,0,0,0,506,505,1,0,0,0,507,
  	508,1,0,0,0,508,506,1,0,0,0,508,509,1,0,0,0,509,108,1,0,0,0,510,512,7,
  	1,0,0,511,510,1,0,0,0,512,513,1,0,0,0,513,511,1,0,0,0,513,514,1,0,0,0,
  	514,110,1,0,0,0,515,519,5,39,0,0,516,518,9,0,0,0,517,516,1,0,0,0,518,
  	521,1,0,0,0,519,520,1,0,0,0,519,517,1,0,0,0,520,522,1,0,0,0,521,519,1,
  	0,0,0,522,523,5,39,0,0,523,112,1,0,0,0,524,525,5,58,0,0,525,114,1,0,0,
  	0,526,527,5,58,0,0,527,528,5,58,0,0,528,116,1,0,0,0,529,530,5,44,0,0,
  	530,118,1,0,0,0,531,532,5,40,0,0,532,120,1,0,0,0,533,534,5,41,0,0,534,
  	122,1,0,0,0,535,536,5,91,0,0,536,124,1,0,0,0,537,538,5,93,0,0,538,126,
  	1,0,0,0,539,540,5,124,0,0,540,544,5,124,0,0,541,542,5,111,0,0,542,544,
  	5,114,0,0,543,539,1,0,0,0,543,541,1,0,0,0,544,128,1,0,0,0,545,546,5,38,
  	0,0,546,551,5,38,0,0,547,548,5,97,0,0,548,549,5,110,0,0,549,551,5,100,
  	0,0,550,545,1,0,0,0,550,547,1,0,0,0,551,130,1,0,0,0,552,556,5,61,0,0,
  	553,554,5,61,0,0,554,556,5,61,0,0,555,552,1,0,0,0,555,553,1,0,0,0,556,
  	132,1,0,0,0,557,558,5,33,0,0,558,562,5,61,0,0,559,560,5,60,0,0,560,562,
  	5,62,0,0,561,557,1,0,0,0,561,559,1,0,0,0,562,134,1,0,0,0,563,564,5,60,
  	0,0,564,136,1,0,0,0,565,566,5,60,0,0,566,567,5,61,0,0,567,138,1,0,0,0,
  	568,569,5,62,0,0,569,140,1,0,0,0,570,571,5,62,0,0,571,572,5,61,0,0,572,
  	142,1,0,0,0,573,574,5,43,0,0,574,144,1,0,0,0,575,576,5,42,0,0,576,146,
  	1,0,0,0,577,578,5,47,0,0,578,148,1,0,0,0,579,580,5,37,0,0,580,150,1,0,
  	0,0,581,586,5,33,0,0,582,583,5,110,0,0,583,584,5,111,0,0,584,586,5,116,
  	0,0,585,581,1,0,0,0,585,582,1,0,0,0,586,152,1,0,0,0,587,589,7,2,0,0,588,
  	587,1,0,0,0,589,590,1,0,0,0,590,588,1,0,0,0,590,591,1,0,0,0,591,592,1,
  	0,0,0,592,593,6,76,0,0,593,154,1,0,0,0,14,0,486,489,494,500,508,513,519,
  	543,550,555,561,585,590,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  poseidonlexerLexerStaticData = staticData.release();
}

}

poseidonLexer::poseidonLexer(CharStream *input) : Lexer(input) {
  poseidonLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *poseidonlexerLexerStaticData->atn, poseidonlexerLexerStaticData->decisionToDFA, poseidonlexerLexerStaticData->sharedContextCache);
}

poseidonLexer::~poseidonLexer() {
  delete _interpreter;
}

std::string poseidonLexer::getGrammarFileName() const {
  return "poseidon.g4";
}

const std::vector<std::string>& poseidonLexer::getRuleNames() const {
  return poseidonlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& poseidonLexer::getChannelNames() const {
  return poseidonlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& poseidonLexer::getModeNames() const {
  return poseidonlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& poseidonLexer::getVocabulary() const {
  return poseidonlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView poseidonLexer::getSerializedATN() const {
  return poseidonlexerLexerStaticData->serializedATN;
}

const atn::ATN& poseidonLexer::getATN() const {
  return *poseidonlexerLexerStaticData->atn;
}




void poseidonLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  poseidonlexerLexerInitialize();
#else
  ::antlr4::internal::call_once(poseidonlexerLexerOnceFlag, poseidonlexerLexerInitialize);
#endif
}