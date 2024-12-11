#include "tests.h"

int main(void) {
  int failed = 0;

  Suite* tests[] = {fsm_test()};
  for (int i = 0; i < 1; i++) {
    SRunner* runner = srunner_create(tests[i]);
    srunner_run_all(runner, CK_NORMAL);
    if (srunner_ntests_failed(runner) != 0) {
      failed = srunner_ntests_failed(runner);
    }
    srunner_free(runner);
  }

  return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}