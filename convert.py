def pairwise(iterable):
    a = iter(iterable)
    return zip(a, a)

raw_data = [5994, 7398, 506, 1662, 478, 1710, 498, 1690, 
  458, 1706, 506, 1686, 538, 1630, 462, 1722, 
  546, 1626, 546, 610, 518, 586, 498, 638, 
  518, 586, 498, 634, 522, 586, 498, 634, 
  522, 586, 498, 1666, 546, 1642, 546, 1622, 
  442, 1742, 546, 1646, 526, 1634, 550, 1642, 
  546, 1622, 514, 642, 518, 590, 494, 638, 
  518, 586, 494, 642, 518, 586, 498, 634, 
  522, 586, 498, 1662, 546, 1646, 542, 1626, 
  482, 1702, 538, 1650, 534, 1630, 542, 1646, 
  538, 1650, 462, 674, 510, 598, 490, 642, 
  510, 594, 530, 602, 514, 594, 534, 598, 
  514, 618, 514, 1626, 442, 1742, 542, 594, 
  486, 1674, 546, 1642, 546, 590, 494, 1666, 
  498, 1690, 494, 614, 502, 630, 494, 1674, 
  466, 682, 478, 630, 446, 1734, 490, 626, 
  442, 682, 486, 630, 466, 1714, 462, 670, 
  482, 1686, 394, 1790, 486, 626, 446, 1738, 
  454, 1730, 486, 1686, 414, 714, 482, 1702, 
  486, 626, 478, 650, 482, 1690, 414, 710, 
  482, 630, 394, 730, 486, 1702, 486, 622, 
  482, 1706, 486, 626, 394, 1786, 486, 630, 
  466, 658, 486, 1702, 482, 630, 450, 1734, 
  482, 634, 462, 1718, 450, 678, 486, 1686, 
  446, 1734, 486, 7422, 482, 1000]


offset_from_end = -27
offset_from_start = 2

header = raw_data[:offset_from_start]
footer = raw_data[offset_from_end:]

bit_threshold = 1000
bits = []

p2_zeros_average = 0
p2_ones_average = 0

zero_count = 0
one_count = 0

for p1, p2 in pairwise(raw_data[offset_from_start:len(raw_data) + offset_from_end]):
    if p2 > bit_threshold:
        p2_ones_average += p2
        bits.append(1)
        one_count += 1
        continue

    bits.append(0)
    p2_zeros_average += p2
    zero_count += 1

size = round(len(raw_data) / 2)
high = p2_ones_average / one_count
low = p2_zeros_average / zero_count

bits_str = "".join(str(x) for x in bits)

print(hex(int(bits_str, 2)))
