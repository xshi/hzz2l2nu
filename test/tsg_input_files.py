''' Forum: HLT Development
Date: Oct 18, 10:31 
From: Andrea Bocci <Andrea Bocci> 
 Hi all,
I've skimmed/collected events at different pileup points and copied them to
the trigger group space on EOS.
There are roughly 50k events per bin (except for the first one),and the
pileup I quote for each group is the result from lumiCalc2.py .

For pu 63, 57 and 44 they come from the July 2012 high-pileup fills:
 - there is no out-of-time pileup;
 - the L1 menu is 2012_v2;
 - the HLT selection is a skim based on all L1-accepted events, *except*
for bits 1-8 (so, no explicit L1_ZeroBias and L1_AlwaysTrue)

For pu 33, 30, 25, 20 and 15 they come from the a couple of Run2012D runs
(207515 and 207518):
 - they have the usual 50ns bunch spacing;
 - the L1 menu is the latest, 2012_v3;
 - the HLT selection is a skim based on all L1-accepted events

I've split the files by lumisections, to make it easier to check the
average pileup with lumiCalc if needed, and so they can be used also for
the DAQ/Hilton tests.

Happy timing,
.Andrea
'''

# pu 63 (run 198609, ls 50, 76k events) - no out-of-time pileup*
pu63 = [ 
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0050_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0050_part2.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0050_part3.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0050_part4.root',
]


# pu 57 (run 198609, ls 107..111, 53k events) **- no out-of-time pileup*

pu57 = [
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0107_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0107_part2.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0108_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0108_part2.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0109_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0109_part2.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0110_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0110_part2.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0111_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0111_part2.root',
]


# pu 44 (run 198603, ls 34..37, 53k events) - no out-of-time pileup*
pu44 = [
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198603/run198603_lumi0034_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198603/run198603_lumi0034_part2.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198603/run198603_lumi0035_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198603/run198603_lumi0035_part2.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198603/run198603_lumi0036_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198603/run198603_lumi0036_part2.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198603/run198603_lumi0037_part1.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198603/run198603_lumi0037_part2.root',
]



# pu 33 (run 207515, ls 81..116, 51k events) - 50 ns bunch spacing*
pu33_50ns = [
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi81.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi82.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi83.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi84.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi85.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi86.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi87.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi88.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi89.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi90.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi91.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi92.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi93.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi94.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi95.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi96.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi97.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi98.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi99.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi100.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi101.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi102.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi103.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi104.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi105.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi106.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi107.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi108.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi109.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi112.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi113.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi114.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi115.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi116.root',
]

# pu 30 (run 207515, ls 183..218, 50k events) - 50 ns bunch spacing*
pu30_50ns = [
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi183.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi184.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi185.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi186.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi187.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi188.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi189.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi190.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi191.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi192.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi193.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi194.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi195.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi196.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi197.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi198.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi199.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi200.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi201.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi202.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi203.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi204.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi205.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi206.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi207.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi208.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi211.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi212.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi213.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi214.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi215.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi216.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi217.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi218.root',
]


# pu 25 (run 207515, ls 429..468, 50k events) - 50 ns bunch spacing*
pu25_50ns = [
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi429.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi430.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi431.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi432.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi433.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi434.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi435.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi436.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi437.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi438.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi439.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi440.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi441.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi442.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi443.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi444.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi445.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi446.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi447.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi448.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi449.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi450.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi451.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi452.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi453.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi454.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi455.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi456.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi457.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi458.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi459.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi460.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi461.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi462.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi463.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi464.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi465.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi466.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi467.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi468.root',
]

# pu 20 (run 207515, ls 789..841, 50k events) - 50 ns bunch spacing*

pu_20_50ns = [
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi789.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi790.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi791.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi792.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi793.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi794.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi795.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi796.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi797.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi798.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi799.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi800.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi801.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi802.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi803.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi804.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi805.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi806.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi807.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi808.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi809.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi810.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi811.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi812.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi813.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi814.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi815.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi816.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi817.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi818.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi819.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi820.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi821.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi822.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi823.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi824.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi825.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi826.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi827.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi828.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi829.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi830.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi831.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi832.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi833.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi834.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi835.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi836.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi837.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi838.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi839.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi840.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207515/run207515_lumi841.root',
]


# pu 15 (run 207518, ls 23..83, 50k events) - 50 ns bunch spacing*
pu_15_50ns = [
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi23.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi24.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi25.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi26.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi27.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi28.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi29.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi30.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi31.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi32.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi33.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi34.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi35.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi36.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi37.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi38.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi39.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi40.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi41.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi42.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi43.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi44.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi45.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi46.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi47.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi48.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi49.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi50.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi51.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi52.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi53.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi54.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi55.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi56.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi57.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi58.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi59.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi61.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi62.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi63.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi64.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi65.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi66.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi67.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi68.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi69.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi70.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi71.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi72.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi73.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi74.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi75.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi76.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi77.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi78.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi79.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi80.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi81.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi82.root',
'/store/group/comm_trigger/TriggerStudiesGroup/Timing/run207518/run207518_lumi83.root',
]


