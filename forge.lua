
variant = variant or 'debug';

local forge = require 'forge.cc' {    
    identifier = 'cc_${platform}_${architecture}';
    platform = forge:operating_system();
    bin = forge:root( ('%s/bin'):format(variant) );
    lib = forge:root( ('%s/lib'):format(variant) );
    obj = forge:root( ('%s/obj'):format(variant) );
    include_directories = {
        forge:root();
    };
    library_directories = {
        forge:root( ('%s/lib'):format(variant) );
    };
    visual_studio = {
        sln = forge:root( 'unittest-cpp.sln' );
    };
    xcode = {
        xcodeproj = forge:root( 'unittest-cpp.xcodeproj' );
    };

    architecture = 'x86_64';
    assertions = variant ~= 'shipping';
    debug = variant ~= 'shipping';
    debuggable = variant ~= 'shipping';
    exceptions = true;
    fast_floating_point = variant ~= 'debug';
    incremental_linking = variant == 'debug';
    link_time_code_generation = variant == 'shipping';
    minimal_rebuild = variant == 'debug';
    optimization = variant ~= 'debug';
    run_time_checks = variant == 'debug';
    runtime_library = variant == 'debug' and 'static_debug' or 'static_release';
    run_time_type_info = true;
    stack_size = 1048576;
    standard = 'c++11';
    string_pooling = variant == 'shipping';
    strip = false;
    warning_level = 3;
    warnings_as_errors = true;
};

-- Bump the C++ standard to c++14 when building on Windows as that is the 
-- closest standard supported by Microsoft Visual C++.
local settings = forge.settings;
if settings.platform == 'windows' then
    settings.standard = 'c++14';
end

buildfile 'unittest-cpp.forge';

forge:all {
    'tests/all',
    'UnitTest++/all'
};
