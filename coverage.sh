mkdir coverage
cd coverage
lcov --directory ../build --capture --output-file coverage.info
lcov --remove coverage.info '*doctest*' '/usr/*' "${HOME}"'/.cache/*' --output-file coverage.info
genhtml coverage.info
lcov --list coverage.info