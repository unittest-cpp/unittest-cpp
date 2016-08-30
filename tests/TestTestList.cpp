#include "UnitTest++/UnitTestPP.h"
#include "UnitTest++/TestList.h"

using namespace UnitTest;

namespace {


   TEST(TestListIsEmptyByDefault)
   {
      TestList list;
      CHECK(list.GetHead() == 0);
   }

   TEST(AddingTestSetsHeadToTest)
   {
      Test test("test");
      TestList list;
      list.Add(&test);

      CHECK(list.GetHead()->m_test == &test);
      CHECK(list.GetHead()->m_next == 0);
   }

   TEST(AddingSecondTestAddsItToEndOfList)
   {
      Test test1("test1");
      Test test2("test2");

      TestList list;
      TestListNode* test1Node = list.Add(&test1);
      TestListNode* test2Node = list.Add(&test2);

      CHECK(list.GetHead()->m_test == &test1);
      CHECK(test1Node->m_next->m_test == &test2);
      CHECK(test2Node->m_next == 0);
   }

   TEST(ListAdderAddsTestToList)
   {
      TestList list;

      Test test("");
      ListAdder adder(list, &test);

      CHECK(list.GetHead()->m_test == &test);
      CHECK(list.GetHead()->m_next == 0);
   }

}
