#include "md.h"
#include "md.c"

static MD_Node *
MakeTestNode(MD_NodeKind kind, MD_String8 string)
{
    return MD_MakeNodeFromString(kind, MD_S8Lit("`TEST_NODE`"), 0, 0, string);
}

static MD_b32
CompareParsedWithTree(MD_String8 string, MD_Node *tree)
{
    MD_ParseResult parse = MD_ParseOneNode(MD_S8Lit(""), string);
    return MD_NodeDeepMatch(tree, parse.node, 0, MD_NodeMatchFlag_Tags | MD_NodeMatchFlag_TagArguments);
}

static struct
{
    int number_of_tests;
    int number_passed;
}
test_ctx;

static void
BeginTest(char *name)
{
    int length = MD_CalculateCStringLength(name);
    int spaces = 25 - length;
    if(spaces < 0)
    {
        spaces = 0;
    }
    printf("\"%s\" %.*s ", name, spaces, "------------------------------");
    test_ctx.number_of_tests = 0;
    test_ctx.number_passed = 0;
}

static void
TestResult(MD_b32 result)
{
    test_ctx.number_of_tests += 1;
    test_ctx.number_passed += !!result;
}

static void
EndTest(void)
{
    printf("[%i/%i] %i tests, %i passed ... ",
           test_ctx.number_passed, test_ctx.number_of_tests,
           test_ctx.number_passed, test_ctx.number_of_tests);
    if(test_ctx.number_of_tests == test_ctx.number_passed)
    {
        printf("SUCCESS");
    }
    else
    {
        printf("FAILED");
    }
    printf("\n");
}

#define Test(name) for(int _i_ = (BeginTest(name), 0); !_i_; _i_ += 1, EndTest())

int main(void)
{
    
    Test("Empty Sets")
    {
        TestResult(CompareParsedWithTree(MD_S8Lit("{}"), MakeTestNode(MD_NodeKind_UnnamedSet, MD_S8Lit(""))));
        TestResult(CompareParsedWithTree(MD_S8Lit("()"), MakeTestNode(MD_NodeKind_UnnamedSet, MD_S8Lit(""))));
        TestResult(CompareParsedWithTree(MD_S8Lit("[]"), MakeTestNode(MD_NodeKind_UnnamedSet, MD_S8Lit(""))));
        TestResult(CompareParsedWithTree(MD_S8Lit("[)"), MakeTestNode(MD_NodeKind_UnnamedSet, MD_S8Lit(""))));
        TestResult(CompareParsedWithTree(MD_S8Lit("(]"), MakeTestNode(MD_NodeKind_UnnamedSet, MD_S8Lit(""))));
    }
    
    Test("Simple Unnamed Sets")
    {
        {
            MD_String8 string = MD_S8Lit("{a, b, c}");
            MD_Node *tree = MakeTestNode(MD_NodeKind_UnnamedSet, MD_S8Lit(""));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("a")));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("b")));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("c")));
            TestResult(CompareParsedWithTree(string, tree));
        }
        {
            MD_String8 string = MD_S8Lit("(1 2 3 4 5)");
            MD_Node *tree = MakeTestNode(MD_NodeKind_UnnamedSet, MD_S8Lit(""));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("1")));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("2")));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("3")));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("4")));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("5")));
            TestResult(CompareParsedWithTree(string, tree));
        }
        {
            MD_String8 string = MD_S8Lit("{a}");
            MD_Node *tree = MakeTestNode(MD_NodeKind_UnnamedSet, MD_S8Lit(""));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("a")));
            TestResult(CompareParsedWithTree(string, tree));
        }
    }
    
    Test("Simple Named Sets")
    {
        MD_String8 string = MD_S8Lit("simple_set: {a, b, c}");
        MD_Node *tree = MakeTestNode(MD_NodeKind_Label, MD_S8Lit("simple_set"));
        MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("a")));
        MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("b")));
        MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("c")));
        TestResult(CompareParsedWithTree(string, tree));
    }
    
    Test("Nested Sets")
    {
        {
            MD_String8 string = MD_S8Lit("{a b:{1 2 3} c}");
            MD_Node *tree = MakeTestNode(MD_NodeKind_UnnamedSet, MD_S8Lit(""));
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("a")));
            {
                MD_Node *sub = MakeTestNode(MD_NodeKind_Label, MD_S8Lit("b"));
                MD_PushChild(sub, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("1")));
                MD_PushChild(sub, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("2")));
                MD_PushChild(sub, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("3")));
                MD_PushChild(tree, sub);
            }
            MD_PushChild(tree, MakeTestNode(MD_NodeKind_Label, MD_S8Lit("c")));
            TestResult(CompareParsedWithTree(string, tree));
        }
    }
    
    Test("Non-Sets")
    {
        TestResult(CompareParsedWithTree(MD_S8Lit("foo"), MakeTestNode(MD_NodeKind_Label, MD_S8Lit("foo"))));
        TestResult(CompareParsedWithTree(MD_S8Lit("123"), MakeTestNode(MD_NodeKind_Label, MD_S8Lit("123"))));
        TestResult(CompareParsedWithTree(MD_S8Lit("+"),   MakeTestNode(MD_NodeKind_Label, MD_S8Lit("+"))));
    }
    
    return 0;
}