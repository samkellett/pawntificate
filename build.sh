#!/bin/bash -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

usage() {
  echo "Usage: $0 [-t <release|debug>] [-c] [-n]" 1>&2
  exit 0
}

# defaults
BUILD_TYPE="release"
CLEAN=0
TESTS=1

while getopts ":t:cn" O; do
  case "${O}" in
    t)
      BUILD_TYPE="$OPTARG"
      ((BUILD_TYPE == "release" || BUILD_TYPE == "debug")) || usage
      ;;
    c)
      CLEAN=1
      ;;
    n)
      TESTS=0
      ;;
    *)
      usage
      ;;
  esac
done

echo "Build Type: ${BUILD_TYPE}"
echo "Clean Build: ${CLEAN}"

BUILD_DIR="$PROJECT_DIR/$BUILD_TYPE"
VENV_DIR="$BUILD_DIR/env"
if [[ $CLEAN == 1 ]]; then
	echo -n "Removing $BUILD_DIR... press 'y' to confirm: "
	read CONFIRM
	if [[ "$CONFIRM" == "y" ]]; then
		rm -r "$BUILD_DIR" || true
	else
		exit 1
	fi
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake "$PROJECT_DIR" \
	-GNinja \
	-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=On \
	-DCMAKE_CXX_COMPILER=clang++ \
	-DCMAKE_C_COMPILER=clang

ninja

if [[ $TESTS == 1 ]]; then
  export CTEST_OUTPUT_ON_FAILURE=1
  export CTEST_PARALLEL_LEVEL=$(sysctl -n hw.ncpu)
  ninja test
fi
