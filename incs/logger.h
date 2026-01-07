#ifndef LOGGER_H
# define LOGGER_H

#if defined(__GNUC__) || defined(__clang__)
  # define FT_API __attribute__((visibility("default")))
#else
  # define FT_API
#endif

#endif
