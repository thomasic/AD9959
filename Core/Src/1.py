import numpy as np
import json

# 参数设置
groups = 51
length_per_group = 20
min_val = 0
max_val = 4096

# 计算振幅和偏移
amplitude = (max_val - min_val) / 2
offset = (max_val + min_val) / 2

# 生成连续的相位数组
# 总长度 = 51 * 20 = 1020
total_length = groups * length_per_group
# 生成 0 到 2*pi*51 的相位，确保连续
phases = np.linspace(0, 2 * np.pi * groups, total_length, endpoint=False)

# 计算正弦值
sine_values = amplitude * np.sin(phases) + offset

# 转换为整数并分割成 51 组
sine_integers = sine_values.astype(int).tolist()
grouped_data = [sine_integers[i:i+length_per_group] for i in range(0, total_length, length_per_group)]

# 打印前3组作为示例
print("前3组数据预览：")
for i in range(51):
    print(f"{grouped_data[i]}")

# 如果需要保存为 JSON 文件，可以取消下面的注释
# with open("sine_data_51.json", "w") as f:
#     json.dump(grouped_data, f)