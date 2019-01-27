#!/bin/bash
set -ex
shopt -s extglob

# Sanitize conda's flags to treat the prefix as system headers (and don't report compiler warnings there.)
export CFLAGS=`echo $CFLAGS | sed 's/ -I/ -isystem/'`
export CPPFLAGS=`echo $CPPFLAGS | sed 's/ -I/ -isystem/'`
export CXXFLAGS=`echo $CXXFLAGS | sed 's/ -I/ -isystem/'`

export CPPFLAGS="-isystem ${BUILD_PREFIX}/include $CPPFLAGS"
export CXXFLAGS="-O2 -g $CXXFLAGS"

case `$CXX --version` in
    *GCC*|*gnu-c++*)
        export EXTRA_CXXFLAGS="-Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wundef -fdiagnostics-show-option -Wconversion"
        ;;
    *clang*)
        export EXTRA_CXXFLAGS="-Werror -Weverything -Wno-padded -Wno-exit-time-destructors -Wno-undefined-func-template -Wno-global-constructors -Wno-c++98-compat -Wno-missing-prototypes"
        ;;
    *)
        $CXX --version
        echo "Which compiler is this?"
        false;
esac

case `uname` in
    Darwin)
        export EXTRA_CXXFLAGS="$EXTRA_CXXFLAGS -fno-common"
        ;;
esac

./bootstrap
./configure --prefix="$PREFIX" CXXFLAGS="$CXXFLAGS" CXX=$CXX || (cat config.log; exit 1)
make -j$CPU_COUNT CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
make install
