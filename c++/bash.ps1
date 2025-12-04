# @author 巷北
# @time 2025.12.3 22:27
function Run-Project{
    if (-not (Test-Path build)) {
        New-Item -ItemType Directory build | Out-Null 
    }

    cd build
    cmake `
        -DCMAKE_BUILD_TYPE=Release `
        -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
        -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
        ..
    cmake --build .
    move-item .\compile_commands.json ..\ -force
    & .\MyCppApp.exe
    cd ..
}

function Debug-Project{
    if (-not (Test-Path build)) {
        New-Item -ItemType Directory build | Out-Null 
    }

    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cmake --build .
    move-item .\compile_commands.json ..\ -force
    & gdb .\MyCppApp.exe
    cd ..
}

function Update-Project{
    if (-not (Test-Path build)) {
        New-Item -ItemType Directory build | Out-Null 
    }

    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cd ..
}

function Delete-All{
    if (Test-Path build) {
        Remove-Item build -Recurse -Force
    }
    if (Test-Path compile_commands.json) {
        Remove-Item compile_commands.json -Force
    }

    if (Test-Path .cache) {
        Remove-Item .cache -Recurse -Force
    }
}

function Invoke-Project{
# 哎, 终于行了. 发现, 原来是powershell脚本跟nanji脚本有冲突...
# 下面的if else 语句没办法, 只能这么写, 要不然总会出问题...
    param(
        [Alias("N")]
        [string]$Name = "MyCppApp",

        [Alias("M")]
        [ValidateSet("Debug", "Release", "Update", "Delete")]
        [string]$Mode = "Release",

        [Alias("C")]
        [ValidateSet("C", "CXX", "C_CXX")]
        [string]$Compiler = "C"
    )
    if (-not (Test-Path build)) {
        New-Item -ItemType Directory build | Out-Null 
    }

    cd build
    if($Mode -eq "Update"){
        if($Compiler -eq "C"){
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                ..
        }
        elseif($Compiler -eq "CXX"){
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        else{
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        move-item .\compile_commands.json ..\ -force
        cd..
    }
    elseif($Mode -eq "Release"){
        if($Compiler -eq "C"){
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                ..
        }
        elseif($Compiler -eq "CXX"){
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        else{
            cmake `
                -DCMAKE_BUILD_TYPE=Release `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        cmake --build .
        move-item .\compile_commands.json ..\ -force
        & .\$Name.exe
        cd..
    }
    elseif($Mode -eq "Debug"){
        if($Compiler -eq "C"){
            cmake `
                -DCMAKE_BUILD_TYPE=Debug `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                ..
        }
        elseif($Compiler -eq "CXX"){
            cmake `
                -DCMAKE_BUILD_TYPE=Debug `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        else{
            cmake `
                -DCMAKE_BUILD_TYPE=Debug `
                -DCMAKE_C_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" `
                -DCMAKE_CXX_COMPILER="F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" `
                ..
        }
        cmake --build .
        move-item .\compile_commands.json ..\ -force
        & gdb .\$Name.exe 
        cd..
    }
    else{
        cd ..
        if (Test-Path build) {
            Remove-Item build -Recurse -Force
        }

        if (Test-Path compile_commands.json) {
            Remove-Item compile_commands.json -Force
        }

        if (Test-Path .cache) {
            Remove-Item .cache -Recurse -Force
        }
    }
}













