#include <iostream>
#include <vector>

using namespace std;

class KUnitTest;

typedef std::vector<KUnitTest *> UnitTests;
typedef UnitTests::iterator UnitTestIterator;

class KUnitTest
{
public:
    KUnitTest(const char *name);
    virtual ~KUnitTest();

    void runTest();

    // Abstract function, to enforce unit test implementation by derived classes
    virtual void runTestImpl() = 0;

    std::string name() const;

private:
    const std::string m_name;
};

static UnitTests *g_allTests = nullptr;

void registerUnitTest(KUnitTest *unitTest)
{
    if (!g_allTests )
        g_allTests = new UnitTests();

    g_allTests->push_back(unitTest);
}

void runAllTests(void)
{
    if (!g_allTests) {
        std::cout << "No tests registered" << endl;
        return;
    }

    for (UnitTestIterator it = g_allTests->begin(); it != g_allTests->end(); ++it) {
        static_cast<KUnitTest *>(*it)->runTest();
    }
}

KUnitTest::KUnitTest(const char *name)  :
    m_name(name)
{
    registerUnitTest(this);
}

KUnitTest::~KUnitTest()
{
}

void KUnitTest::runTest()
{
    std::cout << name() << ": start" << endl;

    runTestImpl();

    std::cout << name() << ": end" << endl;
}

string KUnitTest::name() const
{
    return m_name;
}

#define TEST(x) \
    class KUnitTest##x : public KUnitTest { \
    public: \
        KUnitTest##x() : KUnitTest(#x) { } \
        void runTestImpl(); \
    }; \
    static KUnitTest##x x; \
    void KUnitTest##x::runTestImpl()

TEST(Hello)
{
    std::cout << "In Test: '" << name() << "'" << endl;
}

TEST(MyWorld)
{
    std::cout << "In Test: '" << name() << "'" << endl;
}

int main(int, char *[])
{
    runAllTests();
    return 0;
}
