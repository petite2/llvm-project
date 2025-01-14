#ifndef TSAN_INTERCEPTORS_H
#define TSAN_INTERCEPTORS_H

#include "sanitizer_common/sanitizer_stacktrace.h"
#include "tsan_rtl.h"

namespace __tsan {

class ScopedInterceptor {
 public:
  ScopedInterceptor(ThreadState *thr, const char *fname, uptr pc);
  ~ScopedInterceptor();
  void DisableIgnores() {
    if (UNLIKELY(ignoring_))
      DisableIgnoresImpl();
  }
  void EnableIgnores() {
    if (UNLIKELY(ignoring_))
      EnableIgnoresImpl();
  }

 private:
  ThreadState *const thr_;
  bool in_ignored_lib_;
  bool ignoring_;

  void DisableIgnoresImpl();
  void EnableIgnoresImpl();
};

LibIgnore *libignore();

#if !SANITIZER_GO
inline bool in_symbolizer() {
  return UNLIKELY(cur_thread_init()->in_symbolizer);
}
#endif

}  // namespace __tsan

#define SCOPED_INTERCEPTOR_RAW(func, ...)            \
  ThreadState *thr = cur_thread_init();              \
  ScopedInterceptor si(thr, #func, GET_CALLER_PC()); \
  UNUSED const uptr pc = GET_CURRENT_PC();

#define SCOPED_TSAN_INTERCEPTOR(func, ...)                                \
  SCOPED_INTERCEPTOR_RAW(func, __VA_ARGS__);                              \
  DCHECK(REAL(func));                                                     \
  if (!thr->is_inited || thr->ignore_interceptors || thr->in_ignored_lib) \
    return REAL(func)(__VA_ARGS__);

#define SCOPED_TSAN_INTERCEPTOR_USER_CALLBACK_START() \
    si.DisableIgnores();

#define SCOPED_TSAN_INTERCEPTOR_USER_CALLBACK_END() \
    si.EnableIgnores();

#define TSAN_INTERCEPTOR(ret, func, ...) INTERCEPTOR(ret, func, __VA_ARGS__)

#if SANITIZER_NETBSD
# define TSAN_INTERCEPTOR_NETBSD_ALIAS(ret, func, ...) \
  TSAN_INTERCEPTOR(ret, __libc_##func, __VA_ARGS__) \
  ALIAS(WRAPPER_NAME(pthread_##func));
# define TSAN_INTERCEPTOR_NETBSD_ALIAS_THR(ret, func, ...) \
  TSAN_INTERCEPTOR(ret, __libc_thr_##func, __VA_ARGS__) \
  ALIAS(WRAPPER_NAME(pthread_##func));
# define TSAN_INTERCEPTOR_NETBSD_ALIAS_THR2(ret, func, func2, ...) \
  TSAN_INTERCEPTOR(ret, __libc_thr_##func, __VA_ARGS__) \
  ALIAS(WRAPPER_NAME(pthread_##func2));
#else
# define TSAN_INTERCEPTOR_NETBSD_ALIAS(ret, func, ...)
# define TSAN_INTERCEPTOR_NETBSD_ALIAS_THR(ret, func, ...)
# define TSAN_INTERCEPTOR_NETBSD_ALIAS_THR2(ret, func, func2, ...)
#endif

#endif  // TSAN_INTERCEPTORS_H
