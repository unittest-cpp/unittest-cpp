#ifndef UNITTEST_TESTLIST_H
#define UNITTEST_TESTLIST_H

#include "HelperMacros.h"

namespace UnitTest {

   class Test;

   class UNITTEST_LINKAGE TestListNode
   {
   public:
      TestListNode(Test* test);
      Test* m_test;
      TestListNode* m_next;
   };

   class UNITTEST_LINKAGE TestList
   {
   public:
      TestList();

      TestListNode* Add(Test* test);
      TestListNode* GetHead() const;

   private:
      TestListNode* m_head;
      TestListNode* m_tail;
   };


   class UNITTEST_LINKAGE ListAdder
   {
   public:
      ListAdder(TestList& list, Test* test);
   };

}


#endif
