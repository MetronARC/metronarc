<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="pinhead" urn="urn:adsk.eagle:library:325">
<description>&lt;b&gt;Pin Header Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="1X03" urn="urn:adsk.eagle:footprint:22340/1" library_version="4">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-3.175" y1="1.27" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.635" x2="1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.27" x2="-3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-0.635" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.635" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.27" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0.635" x2="3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-0.635" x2="3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="1.27" y2="-0.635" width="0.1524" layer="21"/>
<pad name="1" x="-2.54" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="0" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="3" x="2.54" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="-3.8862" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-0.254" y1="-0.254" x2="0.254" y2="0.254" layer="51"/>
<rectangle x1="-2.794" y1="-0.254" x2="-2.286" y2="0.254" layer="51"/>
<rectangle x1="2.286" y1="-0.254" x2="2.794" y2="0.254" layer="51"/>
</package>
<package name="1X03/90" urn="urn:adsk.eagle:footprint:22341/1" library_version="4">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-3.81" y1="-1.905" x2="-1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-1.905" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-3.81" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="6.985" x2="-2.54" y2="1.27" width="0.762" layer="21"/>
<wire x1="-1.27" y1="-1.905" x2="1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-1.905" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.635" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="6.985" x2="0" y2="1.27" width="0.762" layer="21"/>
<wire x1="1.27" y1="-1.905" x2="3.81" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-1.905" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0.635" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="6.985" x2="2.54" y2="1.27" width="0.762" layer="21"/>
<pad name="1" x="-2.54" y="-3.81" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="0" y="-3.81" drill="1.016" shape="long" rot="R90"/>
<pad name="3" x="2.54" y="-3.81" drill="1.016" shape="long" rot="R90"/>
<text x="-4.445" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="5.715" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-2.921" y1="0.635" x2="-2.159" y2="1.143" layer="21"/>
<rectangle x1="-0.381" y1="0.635" x2="0.381" y2="1.143" layer="21"/>
<rectangle x1="2.159" y1="0.635" x2="2.921" y2="1.143" layer="21"/>
<rectangle x1="-2.921" y1="-2.921" x2="-2.159" y2="-1.905" layer="21"/>
<rectangle x1="-0.381" y1="-2.921" x2="0.381" y2="-1.905" layer="21"/>
<rectangle x1="2.159" y1="-2.921" x2="2.921" y2="-1.905" layer="21"/>
</package>
</packages>
<packages3d>
<package3d name="1X03" urn="urn:adsk.eagle:package:22458/2" type="model" library_version="4">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X03"/>
</packageinstances>
</package3d>
<package3d name="1X03/90" urn="urn:adsk.eagle:package:22459/2" type="model" library_version="4">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X03/90"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="PINHD3" urn="urn:adsk.eagle:symbol:22339/1" library_version="4">
<wire x1="-6.35" y1="-5.08" x2="1.27" y2="-5.08" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-5.08" x2="1.27" y2="5.08" width="0.4064" layer="94"/>
<wire x1="1.27" y1="5.08" x2="-6.35" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="5.08" x2="-6.35" y2="-5.08" width="0.4064" layer="94"/>
<text x="-6.35" y="5.715" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-7.62" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="-2.54" y="0" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="3" x="-2.54" y="-2.54" visible="pad" length="short" direction="pas" function="dot"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PINHD-1X3" urn="urn:adsk.eagle:component:22524/4" prefix="JP" uservalue="yes" library_version="4">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="A" symbol="PINHD3" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1X03">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22458/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="92" constant="no"/>
</technology>
</technologies>
</device>
<device name="/90" package="1X03/90">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22459/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="17" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="3 pin Terminal Block">
<packages>
<package name="691102710003">
<description>&lt;b&gt;WR-TBL Serie 102 - 5.00 mm Horiz. Entry Modular
&lt;br&gt;&lt;/b&gt;Max Height = 9.9mm , Pitch 5mm , 3 Pins</description>
<text x="0" y="5" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="-3.5" y="-5.5" size="1.27" layer="27">&gt;VALUE</text>
<polygon width="0.1" layer="39">
<vertex x="-8.4" y="4.5"/>
<vertex x="8.4" y="4.5"/>
<vertex x="8.4" y="-3.6"/>
<vertex x="-8.4" y="-3.6"/>
</polygon>
<circle x="-5.8" y="1.4" radius="0.1" width="0.3" layer="21"/>
<wire x1="-7.5" y1="-3.3" x2="7.5" y2="-3.3" width="0.1" layer="51"/>
<wire x1="7.5" y1="4.2" x2="-7.5" y2="4.2" width="0.1" layer="51"/>
<wire x1="-7.5" y1="4.2" x2="-7.5" y2="-0.9" width="0.1" layer="51"/>
<wire x1="-7.5" y1="-1.6" x2="-7.5" y2="-3.3" width="0.1" layer="51"/>
<wire x1="7.5" y1="4.2" x2="7.5" y2="3.25" width="0.1" layer="51"/>
<wire x1="7.5" y1="2.4" x2="7.5" y2="-3.3" width="0.1" layer="51"/>
<wire x1="8.1" y1="2.35" x2="8.1" y2="3.3" width="0.1" layer="51"/>
<wire x1="7.5" y1="3.25" x2="8.1" y2="3.3" width="0.1" layer="51"/>
<wire x1="7.5" y1="2.4" x2="8.1" y2="2.35" width="0.1" layer="51"/>
<wire x1="-8.1" y1="-0.825" x2="-8.1" y2="-1.675" width="0.1" layer="51"/>
<wire x1="-7.5" y1="-1.6" x2="-8.1" y2="-1.675" width="0.1" layer="51"/>
<wire x1="-7.5" y1="-0.9" x2="-8.1" y2="-0.825" width="0.1" layer="51"/>
<wire x1="-7.6" y1="4.3" x2="7.6" y2="4.3" width="0.2" layer="21"/>
<wire x1="7.6" y1="4.3" x2="7.6" y2="3.35" width="0.2" layer="21"/>
<wire x1="7.6" y1="3.35" x2="8.2" y2="3.4" width="0.2" layer="21"/>
<wire x1="8.2" y1="3.4" x2="8.2" y2="2.25" width="0.2" layer="21"/>
<wire x1="8.2" y1="2.25" x2="7.6" y2="2.3" width="0.2" layer="21"/>
<wire x1="7.6" y1="2.3" x2="7.6" y2="-3.4" width="0.2" layer="21"/>
<wire x1="7.6" y1="-3.4" x2="-7.6" y2="-3.4" width="0.2" layer="21"/>
<wire x1="-7.6" y1="-3.4" x2="-7.6" y2="-1.7" width="0.2" layer="21"/>
<wire x1="-7.6" y1="-1.7" x2="-8.2" y2="-1.775" width="0.2" layer="21"/>
<wire x1="-8.2" y1="-1.775" x2="-8.2" y2="-0.725" width="0.2" layer="21"/>
<wire x1="-8.2" y1="-0.725" x2="-7.6" y2="-0.8" width="0.2" layer="21"/>
<wire x1="-7.6" y1="-0.8" x2="-7.6" y2="4.3" width="0.2" layer="21"/>
<pad name="2" x="0" y="0" drill="1.3"/>
<pad name="1" x="-5" y="0" drill="1.3"/>
<pad name="3" x="5" y="0" drill="1.3"/>
</package>
</packages>
<symbols>
<symbol name="3PINS_ENTRY">
<wire x1="-3.81" y1="0" x2="3.81" y2="0" width="0.254" layer="94"/>
<wire x1="3.81" y1="-2.54" x2="-3.81" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-3.81" y1="-2.54" x2="-3.81" y2="0" width="0.254" layer="94"/>
<wire x1="3.81" y1="0" x2="3.81" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-1.778" x2="-2.54" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-1.778" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-2.286" y1="-1.016" x2="-2.794" y2="-1.524" width="0.1524" layer="94"/>
<wire x1="0.254" y1="-1.016" x2="-0.254" y2="-1.524" width="0.1524" layer="94"/>
<wire x1="2.54" y1="-1.778" x2="2.54" y2="-2.54" width="0.254" layer="94"/>
<wire x1="2.794" y1="-1.016" x2="2.286" y2="-1.524" width="0.1524" layer="94"/>
<text x="-4.88" y="-0.922" size="1.016" layer="95" align="bottom-right">&gt;NAME</text>
<text x="-4.83" y="-2.58" size="1.016" layer="96" align="bottom-right">&gt;VALUE</text>
<circle x="-2.54" y="-1.27" radius="0.508" width="0.254" layer="94"/>
<circle x="0" y="-1.27" radius="0.508" width="0.254" layer="94"/>
<circle x="2.54" y="-1.27" radius="0.508" width="0.254" layer="94"/>
<pin name="1" x="-2.54" y="-5.08" visible="pad" length="short" direction="pas" rot="R90"/>
<pin name="2" x="0" y="-5.08" visible="pad" length="short" direction="pas" rot="R90"/>
<pin name="3" x="2.54" y="-5.08" visible="pad" length="short" direction="pas" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="691102710003" prefix="J">
<description>&lt;b&gt;WR-TBL Serie 102 - 5.00 mm Horiz. Entry Modular&lt;/b&gt;
&lt;br&gt;
&lt;br&gt;&lt;b&gt;
KIND PROPERTIES &lt;/b&gt;
&lt;br&gt;&lt;br&gt;
Pitch 5 mm 
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;MATERIAL PROPERTIES &lt;/b&gt;
&lt;br&gt;
&lt;br&gt;Insulator Material PA66 
&lt;br&gt;Insulator Flammability Rating UL94 V-0 
&lt;br&gt;Insulator Color Blue 
&lt;br&gt;Contact Material Brass 
&lt;br&gt;Contact Plating Copper over Zinc 
&lt;br&gt;Terminal Screw Material Steel 
&lt;br&gt;Terminal Screw Plating Zinc 
&lt;br&gt;Wire Guard Material Stainless Steel 
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;GENERAL INFORMATION &lt;/b&gt;
&lt;br&gt;
&lt;br&gt;Operating Temperature -40 up to +105 °C 
&lt;br&gt;Compliance Lead free / RoHS 
&lt;br&gt;&lt;br&gt;
&lt;b&gt;ELECTRICAL PROPERTIES &lt;/b&gt;
&lt;br&gt;
&lt;br&gt;IR cULus 15 A 
&lt;br&gt;IR VDE 17.5 A 
&lt;br&gt;Working Voltage [cULus] 300 V (AC) 
&lt;br&gt;Working Voltage [VDE] 250 V (AC) 
&lt;br&gt;Withstanding Voltage [UL] 1600 V (AC) 
&lt;br&gt;Withstanding Voltage [cULus] 1600 V (AC) 
&lt;br&gt;Withstanding Voltage [VDE] 2000 V (AC) 
&lt;br&gt;Contact Resistance [UL] 20 mΩ 
&lt;br&gt;Contact Resistance [cULus] 20 mΩ 
&lt;br&gt;Contact Resistance [VDE] 20 mΩ 
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;MECHANICAL PROPERTIES &lt;/b&gt;
&lt;br&gt;
&lt;br&gt;Wire Strip Length 5 up to 6 (mm) 
&lt;br&gt;Screw M2.6 
&lt;br&gt;Solid Wire Section (AWG) 22 to 14 (AWG) 
&lt;br&gt;Solid Wire Section (Metric) 0.326 to 2.08 (mm) 
&lt;br&gt;Stranded Wire Section (AWG) 22 to 14 (AWG) 
&lt;br&gt;Stranded Wire Section (Metric) 0.326 to 2.08 (mm) 
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;CERTIFICATION &lt;/b&gt;
&lt;br&gt;
&lt;br&gt;cULus Approval E315414 
&lt;br&gt;UL Approval E315414 
&lt;br&gt;VDE Approval 40022612 
&lt;br&gt;VDE Approval 40022612 Model number 102 
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;PACKAGING PROPERTIES &lt;/b&gt;
&lt;br&gt;
&lt;br&gt;Packaging Box 
&lt;br&gt;
&lt;br&gt;
&lt;br&gt;&lt;a href="https://katalog.we-online.com/media/images/v2/o33225v209%20Family_WR-TBL_69110271000x.jpg" title="Enlarge picture"&gt;
&lt;img src="https://katalog.we-online.com/media/images/v2/o33225v209%20Family_WR-TBL_69110271000x.jpg" width="320"&gt;&lt;/a&gt;&lt;p&gt;
Details see: &lt;a href="https://katalog.we-online.com/en/em/TBL_5_00_102_6911027100XX"&gt;https://katalog.we-online.com/en/em/TBL_5_00_102_6911027100XX&lt;/a&gt;&lt;p&gt;
&lt;/b&gt;Updated by Ella Wu  2019-11-07&lt;br&gt;
&lt;/b&gt;2019(C) Wurth Elektronik  &lt;a href="https://pricing.snapeda.com/parts/691102710003/W%C3%BCrth%20Elektronik/view-part?ref=eda"&gt;Check availability&lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="3PINS_ENTRY" x="0" y="0"/>
</gates>
<devices>
<device name="" package="691102710003">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="AVAILABILITY" value="In Stock"/>
<attribute name="CHECK_PRICES" value="https://www.snapeda.com/parts/691102710003/Wurth+Elektronik/view-part/?ref=eda"/>
<attribute name="DESCRIPTION" value=" 3 Position Wire to Board Terminal Block Horizontal with Board 0.197 (5.00mm) Through Hole "/>
<attribute name="MF" value="Würth Elektronik"/>
<attribute name="MP" value="691102710003"/>
<attribute name="PACKAGE" value="None"/>
<attribute name="PRICE" value="None"/>
<attribute name="PURCHASE-URL" value="https://www.snapeda.com/api/url_track_click_mouser/?unipart_id=4870208&amp;manufacturer=Würth Elektronik&amp;part_name=691102710003&amp;search_term=terminal block 3 pin"/>
<attribute name="SNAPEDA_LINK" value="https://www.snapeda.com/parts/691102710003/Wurth+Elektronik/view-part/?ref=snap"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="OUT" library="pinhead" library_urn="urn:adsk.eagle:library:325" deviceset="PINHD-1X3" device="" package3d_urn="urn:adsk.eagle:package:22458/2"/>
<part name="INPUT" library="3 pin Terminal Block" deviceset="691102710003" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="OUT" gate="A" x="17.78" y="83.82" smashed="yes">
<attribute name="NAME" x="11.43" y="89.535" size="1.778" layer="95"/>
<attribute name="VALUE" x="11.43" y="76.2" size="1.778" layer="96"/>
</instance>
<instance part="INPUT" gate="G$1" x="35.56" y="86.36" smashed="yes">
<attribute name="NAME" x="30.68" y="85.438" size="1.016" layer="95" align="bottom-right"/>
<attribute name="VALUE" x="30.73" y="83.78" size="1.016" layer="96" align="bottom-right"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="24V" class="0">
<segment>
<pinref part="OUT" gate="A" pin="1"/>
<wire x1="15.24" y1="86.36" x2="12.7" y2="86.36" width="0.1524" layer="91"/>
<label x="7.62" y="86.36" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="INPUT" gate="G$1" pin="1"/>
<wire x1="33.02" y1="81.28" x2="33.02" y2="78.74" width="0.1524" layer="91"/>
<label x="30.48" y="78.74" size="1.778" layer="95"/>
</segment>
</net>
<net name="0V" class="0">
<segment>
<pinref part="OUT" gate="A" pin="2"/>
<wire x1="15.24" y1="83.82" x2="12.7" y2="83.82" width="0.1524" layer="91"/>
<label x="7.62" y="83.82" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="INPUT" gate="G$1" pin="2"/>
<wire x1="35.56" y1="81.28" x2="35.56" y2="78.74" width="0.1524" layer="91"/>
<label x="33.02" y="76.2" size="1.778" layer="95"/>
</segment>
</net>
<net name="GREEN" class="0">
<segment>
<pinref part="OUT" gate="A" pin="3"/>
<wire x1="15.24" y1="81.28" x2="12.7" y2="81.28" width="0.1524" layer="91"/>
<label x="7.62" y="81.28" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="INPUT" gate="G$1" pin="3"/>
<wire x1="38.1" y1="81.28" x2="38.1" y2="78.74" width="0.1524" layer="91"/>
<label x="38.1" y="78.74" size="1.778" layer="95"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
