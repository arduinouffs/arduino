def pairwise(iterable):
    a = iter(iterable)
    return zip(a, a)

raw_data = [6062, 7422, 458, 1750, 510, 1678, 506, 1702, 
  506, 1678, 478, 1730, 506, 1682, 474, 1730, 
  510, 1682, 414, 722, 506, 614, 446, 690, 
  510, 614, 474, 662, 510, 610, 510, 630, 
  510, 610, 510, 1698, 506, 1682, 534, 1670, 
  506, 1682, 442, 1758, 514, 1682, 446, 1754, 
  510, 1698, 474, 642, 510, 634, 470, 646, 
  510, 614, 462, 674, 510, 614, 414, 718, 
  510, 638, 394, 1782, 510, 1702, 470, 1714, 
  506, 1702, 506, 1678, 478, 1726, 510, 1682, 
  414, 1786, 510, 634, 442, 674, 510, 614, 
  490, 646, 506, 634, 398, 718, 510, 614, 
  490, 642, 510, 1702, 470, 646, 510, 1698, 
  474, 642, 510, 630, 478, 638, 510, 634, 
  474, 1710, 510, 630, 502, 1686, 498, 638, 
  506, 1682, 474, 1726, 482, 1726, 446, 1738, 
  482, 662, 446, 670, 482, 1726, 482, 1702, 
  454, 686, 506, 1678, 474, 1730, 514, 610, 
  470, 1730, 514, 1694, 478, 638, 518, 626, 
  474, 1710, 482, 658, 506, 610, 550, 1658, 
  510, 610, 450, 686, 510, 1678, 466, 670, 
  510, 1682, 462, 670, 510, 1698, 474, 642, 
  510, 634, 470, 1714, 502, 638, 506, 1678, 
  470, 670, 502, 1690, 462, 670, 502, 1706, 
  442, 1738, 502, 7478, 470, 1000]


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

print(bits_str)
# print(hex(int(bits_str, 2)))
