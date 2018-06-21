pipeline {
	agent any
	parameters {
		string(name: 'SimulatedFailures', defaultValue: '', description: 'space separated components to fail. Possible values are Keyboard Video Power CPU ROM RAM')
	}
	
	environment {
		//For 32-bit TestStand, remove '64'
		TSBin = "%TestStandBin%" 
		TS = "%TestStand%"
		MSXSL = "C:\\MSXSL.exe"
	}

    stages {
		//TODO: consider adding pre-build stage for build go/no-go test. If we archive the previous workspace, we can run the diff merge utility to get a unique changelist
        stage('BuildTSD') {
            steps {
				script{
					try{
						bat "\"${TSBin}\\BuildTSD.exe\" /copyLogFiles \"${WORKSPACE}\" \"${WORKSPACE}\\Computer Motherboard Test Program.tsd\""
					}
					catch(Exception err){
						echo 'Exception caught '+err.getMessage()
						switch (err.getMessage()){
							case 'script returned exit code 1':
								echo 'The TSD file failed to load '
								throw err //rethrow so we stop the pipeline
								break
							case 'script returned exit code 2':
								//Some warnings cause BuildTSD to falsely report that the build was aborted. This is tracked under CAR 650867 and was fixed in TestStand 2017.
								//As a workaround for earlier versions, modify this case to not rethrow the error. Instead, rely on the Log or JUnit report for build status.
								echo 'The TSDU was aborted while building, the installer was not built'
								throw err //rethrow so we stop the pipeline
								break	
							case 'script returned exit code 3':
								echo 'The TSDU encountered a fatal error while building'
								throw err //rethrow so we stop the pipeline
								break
							case 'script returned exit code 4':
								currentBuild.result = 'FAILURE'
								echo 'The TSDU created an installer but there were some errors while building.'
								break
							case 'script returned exit code 5':
								currentBuild.result = 'UNSTABLE'
								echo 'The TSDU created an installer but there were some warnings while building.'
								break
							case 'script returned exit code 6':
								echo ' The TSDU could not be found.'
								throw err //rethrow so we stop the pipeline
								break
							case 'script returned exit code 7':
								echo 'TSDU did not return a value: This can happen if the TSDU crashes before being able to communicate the result to the caller process.'
								throw err //rethrow so we stop the pipeline
								break
							default :
								throw err //rethrow so we stop the pipeline
								break
						}
					}
					//convert build status log to HTML and archive
					bat "${MSXSL} \"${WORKSPACE}\\TSDU Detailed Status Log.xml\" \"${TS}\\Components\\Stylesheets\\TSDU\\TSDULogViewer.xsl\" -o \"${WORKSPACE}\\TSDU Detailed Status Log.html\""
					archiveArtifacts 'TSDU Detailed Status Log.html'
					
					//publish results as JUnit
					bat "${MSXSL} \"${WORKSPACE}\\TSDU Detailed Status Log.xml\" \"${WORKSPACE}\\Stylesheets\\TSDUStatusLog_JUnitSimple.xslt\" -o \"${WORKSPACE}\\TSDU_testcases.xml\""
					junit allowEmptyResults: true, testResults: 'TSDU_testcases.xml'
				}
            }
        }

		stage('Analyzer') {
            steps {
				script{
					try{
						//We want to run the analyzer on the files from the <Image> directory since they've been compiled and linked properly.
						//This is also what our end user will get, so we want to make sure its been tested (not just the our source)
						bat "\"${TSBin}\\AnalyzerApp\" \"${WORKSPACE}\\Image\\target\\Computer Motherboard Analyzer Project.tsaproj\" /analyze /report AnalyzerReport${BUILD_NUMBER}.xml /quit"
					}
					//TODO - test error cases
					catch(Exception err) {
						echo 'Exception caught '+err.getMessage()
						switch (err.getMessage()){
							case 'script returned exit code -2':
								echo 'One or more paths are invalid'
								throw err //rethrow so we stop the pipeline
								break
							case 'script returned exit code -1':
								echo 'One or more command line flags were invalid'
								throw err //rethrow so we stop the pipeline
								break	
							case 'script returned exit code 1':
								currentBuild.result = 'FAILURE'
								echo 'One or more errors reported during analysis'
								break
							case 'script returned exit code 2':
								currentBuild.result = 'UNSTABLE'
								echo 'One or more warnings reported during analysis'
								break
							default :
								throw err //rethrow so we stop the pipeline
								break
						}
					}
					//convert analyzer report to HTML and archive
					bat "${MSXSL} \"${WORKSPACE}\\Image\\target\\AnalyzerReport${BUILD_NUMBER}.xml\" \"${TS}\\Components\\Stylesheets\\Analyzer\\AnalyzerReportViewer.xsl\" -o \"${WORKSPACE}\\Image\\target\\AnalyzerReport${BUILD_NUMBER}.html\""
					archiveArtifacts "Image\\target\\AnalyzerReport${BUILD_NUMBER}.html"
					
					//publish results as JUnit
					bat "${MSXSL} \"${WORKSPACE}\\Image\\target\\AnalyzerReport${BUILD_NUMBER}.xml\" \"${WORKSPACE}\\Stylesheets\\AnalyzerReport_JUnitSimple.xslt\" -o \"${WORKSPACE}\\Analyzer_testcases.xml\""
					junit allowEmptyResults: true, testResults: 'Analyzer_testcases.xml'
				}	
            }
        }

        /*stage('Run Single Execution') {
            steps {
				script{
					try{
						bat "\"${WORKSPACE}\\UserInterface_DotNet\\bin\\Release\\TestExec.exe\" /runEntryPoint \"Single Pass\" \"${WORKSPACE}\\Image\\target\\Computer Motherboard Test Sequence.seq\" /SimulateFailure ${params.SimulatedFailures} /OutputToStdIO /quit"
					}
					catch(Exception err){
						echo 'Exception caught '+err.getMessage()
						switch (err.getMessage()){
							case 'script returned exit code 1':
								echo 'One or more command line errors reported'
								throw err //rethrow so we stop the pipeline
								break
							case 'script returned exit code 2':
								echo 'Sequence Failed'
								break
							case 'script returned exit code 3':
								currentBuild.result = 'FAILURE'
								echo 'Sequence Terminated'
								break
							case 'script returned exit code 4':
								currentBuild.result = 'FAILURE'
								echo 'Sequence Aborted'
								break
							case 'script returned exit code 5':
								currentBuild.result = 'FAILURE'
								echo 'Killed Threads'
								break								
							default :
								throw err //rethrow so we stop the pipeline
								break
						}
					}
					
					//convert XML report to HTML and archive
					bat "${MSXSL} \"${WORKSPACE}\\Image\\target\\Report.xml\" \"${TS}\\Components\\Models\\TestStandModels\\ATML\\StyleSheets\\TR5_Horizontal.xsl\" -o \"${WORKSPACE}\\Image\\target\\TestReport${BUILD_NUMBER}.html\""
					archiveArtifacts "Image\\target\\TestReport${BUILD_NUMBER}.html"
					
					//publish results as JUnit
					bat "${MSXSL} \"${WORKSPACE}\\image\\target\\Report.xml\" \"${WORKSPACE}\\Stylesheets\\ATML_JUnit_Simple.xslt\" -o \"${WORKSPACE}\\testcases.xml\""
					junit allowEmptyResults: true, testResults: 'testcases.xml'
				}
            }*/
        }
        /*stage('Publish') {
            steps {
                echo 'Deploying....'
            }
        }
    }*/
}
