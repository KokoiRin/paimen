set_project("PomodoroTimer")
set_version("1.0.0")
set_languages("c++17")

add_rules("mode.debug", "mode.release")

target("pomodoro")
    set_kind("binary")
    add_rules("qt.widgetapp")
    
    -- 添加源文件
    add_files("src/*.cpp")
    add_files("src/*.h")
    
    -- 添加头文件路径
    add_includedirs("src")
    
    -- 添加Qt模块
    add_frameworks("QtWidgets", "QtGui", "QtCore")
    
    -- Release模式优化
    if is_mode("release") then
        set_optimize("fastest")
        add_defines("NDEBUG", "QT_NO_DEBUG_OUTPUT")
        set_symbols("hidden")
        -- 启用链接时优化
        add_ldflags("/OPT:REF", "/OPT:ICF", {force = true})
    end
    
    -- 性能优化编译选项
    add_cxxflags("/utf-8", {force = true})  -- 使用UTF-8编码避免警告
    
    -- Windows子系统
    if is_plat("windows") then
        add_ldflags("/SUBSYSTEM:WINDOWS", "/ENTRY:mainCRTStartup", {force = true})
    end
