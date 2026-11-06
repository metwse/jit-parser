/* private items */

#ifndef MAS_DETAIL_H
#define MAS_DETAIL_H


#if defined(__GNUC__) || defined(__clang__)
/* warn unused result */
#define _mas_wur __attribute__((warn_unused_result))
/* mark internal functions */
#define _masi __attribute__((visibility("internal"))) extern
#else
#define _mas_wur
#define _masi extern
#endif


#endif
