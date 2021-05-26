let project = new Project('data_desk');

project.cmd = true;
project.kore = false;
project.c11 = true;
if(platform === Platform.Windows){
    project.addDefine('BUILD_WIN32=1');
    project.addDefine('BUILD_LINUX=0');
}
else {
    project.addDefine('BUILD_WIN32=0');
    project.addDefine('BUILD_LINUX=1');
}

project.addFile('source/data_desk_main.c');

project.setDebugDir('Deployment');

resolve(project);
