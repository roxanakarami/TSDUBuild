<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="16008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="CCSymbols" Type="Str">OS,Win;CPU,x86;</Property>
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="SubVIs" Type="Folder">
			<Item Name="Get Engine Reference.vi" Type="VI" URL="../TestExec.llb/Get Engine Reference.vi"/>
			<Item Name="Check for Errors.vi" Type="VI" URL="../TestExec.llb/Check for Errors.vi"/>
			<Item Name="DisplaySequenceFile Event Callback.vi" Type="VI" URL="../TestExec.llb/DisplaySequenceFile Event Callback.vi"/>
			<Item Name="DisplayExecution Event Callback.vi" Type="VI" URL="../TestExec.llb/DisplayExecution Event Callback.vi"/>
			<Item Name="ReportError Event Callback.vi" Type="VI" URL="../TestExec.llb/ReportError Event Callback.vi"/>
			<Item Name="ExitApplication Event Callback.vi" Type="VI" URL="../TestExec.llb/ExitApplication Event Callback.vi"/>
			<Item Name="Display Error.vi" Type="VI" URL="../TestExec.llb/Display Error.vi"/>
		</Item>
		<Item Name="Controls" Type="Folder">
			<Item Name="Basic TestStand UI Data.ctl" Type="VI" URL="../TestExec.llb/Basic TestStand UI Data.ctl"/>
			<Item Name="State Selector.ctl" Type="VI" URL="../TestExec.llb/State Selector.ctl"/>
		</Item>
		<Item Name="Basic User Interface.vi" Type="VI" URL="../TestExec.llb/Basic User Interface.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="TestStand - Get Resource String.vi" Type="VI" URL="/&lt;vilib&gt;/addons/TestStand/_TSUtility.llb/TestStand - Get Resource String.vi"/>
				<Item Name="TestStand - Append Code and Description to Error Source.vi" Type="VI" URL="/&lt;vilib&gt;/addons/TestStand/_TSUISupport.llb/TestStand - Append Code and Description to Error Source.vi"/>
				<Item Name="TestStand - Set TestStand Application Window.vi" Type="VI" URL="/&lt;vilib&gt;/addons/TestStand/_TSUISupport.llb/TestStand - Set TestStand Application Window.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="LabVIEW Test - Sequence Context.ctl" Type="VI" URL="/&lt;vilib&gt;/addons/TestStand/_TSLegacy.llb/LabVIEW Test - Sequence Context.ctl"/>
			</Item>
			<Item Name="Configure Command Connections.vi" Type="VI" URL="../TestExec.llb/Configure Command Connections.vi"/>
			<Item Name="Configure List Connections.vi" Type="VI" URL="../TestExec.llb/Configure List Connections.vi"/>
			<Item Name="ProcessUserCommandLineArguments Event Callback.vi" Type="VI" URL="../TestExec.llb/ProcessUserCommandLineArguments Event Callback.vi"/>
			<Item Name="UIMessage Event Callback.vi" Type="VI" URL="../TestExec.llb/UIMessage Event Callback.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="TestExec.exe" Type="EXE">
				<Property Name="App_INI_aliasGUID" Type="Str">{C758FE44-588C-4476-A58D-6F18CA6BE90B}</Property>
				<Property Name="App_INI_GUID" Type="Str">{BA2F171D-8BAB-4CC1-999E-A24535A2B59B}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{CD8C1697-DB17-4673-8B1D-2A77CF6D77AA}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TestExec.exe</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../Build</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{0B286841-9611-4427-A75D-EBD3941289A1}</Property>
				<Property Name="Bld_version.build" Type="Int">52</Property>
				<Property Name="Bld_version.major" Type="Int">16</Property>
				<Property Name="Bld_version.minor" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TestExec.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../Build/TestExec.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../Build/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[2].destName" Type="Str">Destination Directory</Property>
				<Property Name="Destination[2].path" Type="Path">../Build</Property>
				<Property Name="Destination[2].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">3</Property>
				<Property Name="Exe_cmdLineArgs" Type="Bool">true</Property>
				<Property Name="Source[0].Container.applyDestination" Type="Bool">true</Property>
				<Property Name="Source[0].Container.applyInclusion" Type="Bool">true</Property>
				<Property Name="Source[0].Container.applyPrefix" Type="Bool">true</Property>
				<Property Name="Source[0].Container.applyProperties" Type="Bool">true</Property>
				<Property Name="Source[0].itemID" Type="Str">{6BA18208-676D-40F3-AA3D-BC48AFAE6408}</Property>
				<Property Name="Source[0].newName" Type="Str">Simple OI - </Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Basic User Interface.vi</Property>
				<Property Name="Source[1].properties[0].type" Type="Str">Remove front panel</Property>
				<Property Name="Source[1].properties[0].value" Type="Bool">false</Property>
				<Property Name="Source[1].propertiesCount" Type="Int">1</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 1998-2017</Property>
				<Property Name="TgtF_productName" Type="Str">National Instruments TestStand 2016 SP1</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{C39B9FF6-5BEB-4533-8394-7DC177C58607}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TestExec.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
