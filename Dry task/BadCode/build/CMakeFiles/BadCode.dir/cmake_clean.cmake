file(REMOVE_RECURSE
  "BadCode.exe"
  "BadCode.exe.manifest"
  "BadCode.pdb"
  "CMakeFiles/BadCode.dir/badcode.c.obj"
  "libBadCode.dll.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/BadCode.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
