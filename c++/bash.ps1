# @author 巷北
# @time 2025.12.3 22:27
function Run-Project{
    if (-not (Test-Path build)) {
        New-Item -ItemType Directory build | Out-Null 
    }

    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
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

function Update-Project{
# 哎, 终于行了. 发现, 原来是powershell脚本跟nanji脚本有冲突...
# 下面的if else 语句没办法, 只能这么写, 要不然总会出问题...
    param(
        [Alias("N")]
        [string]$Name = "MyCppApp",

        [Alias("M")]
        [ValidateSet("Debug", "Release", "Update", "Delete")]
        [string]$Mode = "Release"
    )
    if (-not (Test-Path build)) {
        New-Item -ItemType Directory build | Out-Null 
    }

    cd build
    if($Mode -eq "Update"){
        cmake -DCMAKE_BUILD_TYPE="Release" ..
        move-item .\compile_commands.json ..\ -force
    }
    elseif($Mode -eq "Release"){
        cmake -DCMAKE_BUILD_TYPE="Release" ..
        cmake --build .
        move-item .\compile_commands.json ..\ -force
        & .\$Name.exe
    }
    elseif($Mode -eq "Debug"){
        cmake -DCMAKE_BUILD_TYPE="Debug" ..
        cmake --build .
        move-item .\compile_commands.json ..\ -force
        & gdb .\$Name.exe 
    }
    else{
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
    cd..
}













