@echo off

echo ~~~ Metadesk Build ~~~
set compile_flags= /nologo /Zi /FC /MP /I..\source\

if not exist build mkdir build
pushd build
echo.
echo ~~~ Build All Samples ~~~
cl %compile_flags% ..\samples\old_style_custom_layer.c
cl %compile_flags% ..\samples\static_site_generator\static_site_generator.c
cl %compile_flags% ..\samples\output_parse\output_parse.c
echo.
echo ~~~ Build All Tests ~~~
cl %compile_flags% ..\tests\sanity_tests.c
cl %compile_flags% ..\tests\unicode_test.c
cl %compile_flags% ..\tests\cpp_build_test.cpp
popd

echo.
echo ~~~ Running Sanity Tests ~~~
pushd build
sanity_tests.exe
popd

echo.
echo ~~~ Running Static Site Generator Sample ~~~
pushd samples
pushd static_site_generator
pushd example_site
if not exist generated mkdir generated
pushd generated
..\..\..\..\build\static_site_generator.exe --siteinfo ..\site_info.md --pagedir ..\
popd
popd
popd
popd

echo.
echo ~~~ Running Output Parse Sample ~~~
pushd samples
pushd output_parse
pushd examples
if not exist output mkdir output
pushd output
..\..\..\..\build\output_parse.exe ..\example.md ..\example2.md
popd
popd
popd
popd
