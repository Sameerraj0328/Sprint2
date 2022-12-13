#include <bits/stdc++.h>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>
#include "msg_client.h"

using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------------------------

class TestTcpClient : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestTcpClient);
    /*CPPUNIT_TEST(TestinitializeSocket);
     CPPUNIT_TEST(TestsendMsg);*/
     CPPUNIT_TEST(Testclose);
     CPPUNIT_TEST(TestconnectTo);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    
    /*void TestinitializeSocket(void);
    void TestsendMsg(void);*/
    void Testclose(void);
    /*void TestconnectTo(void);*/
    

private:

    TcpClient *msg_client;
};

//-----------------------------------------------------------------------------


/*void TestTcpClient::TestinitializeSocket(void)
{
    CPPUNIT_ASSERT(0 == msg_client->initializeSocket());
    CPPUNIT_ASSERT(-1 == msg_client->initializeSocket());
}
void TestTcpClient::TestsendMsg(void)
{
    CPPUNIT_ASSERT(0 == msg_client->sendMsg());
    CPPUNIT_ASSERT(-1 == msg_client->sendMsg());
}*/
/*void TestTcpClient::TestconnectTo(void)
{
    CPPUNIT_ASSERT(1 == msg_client->connectTo());
    CPPUNIT_ASSERT(0 == msg_client->connectTo());
}*/
void TestTcpClient::Testclose(void)
{
    CPPUNIT_ASSERT(-1 == msg_client->close());
    CPPUNIT_ASSERT(0 == msg_client->close());
}

void TestTcpClient::setUp(void)
{
msg_client = new TcpClient();


}

void TestTcpClient::tearDown(void)
{
delete msg_client;
	}

CPPUNIT_TEST_SUITE_REGISTRATION( TestTcpClient);



//-----------------------------------------------------------------------------


int main(int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    ofstream xmlFileOut("client.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}

