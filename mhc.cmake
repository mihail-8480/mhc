cmake_minimum_required(VERSION 3.20)
function(mhc_feature name impl)
    string(TOUPPER ${name} name_upper)
    target_compile_definitions(mhc_base INTERFACE "MH_${name_upper}_IMPL=\"${impl}\"")
endfunction()

function(mhc_module_declare name)
    string(TOUPPER ${name} name_upper)
    target_compile_options("mhc_${name}" PUBLIC "-fPIC")
    target_link_libraries("mhc_${name}" PRIVATE mhc_base)
    target_link_libraries(mhc_base INTERFACE "mhc_${name}")
    target_include_directories("mhc_${name}" PUBLIC include/)
    target_include_directories("mhc_${name}" PRIVATE private/)
    target_link_libraries(mhc PUBLIC "mhc_${name}")
    target_compile_definitions(mhc_base INTERFACE "MH_MODULE_${name}")
endfunction()

function(mhc_enable name)
    include("modules/${name}.cmake")
endfunction()