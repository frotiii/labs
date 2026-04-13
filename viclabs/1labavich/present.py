import matplotlib.pyplot as plt
import numpy as np

# Читаем данные
data = np.loadtxt('angular_momentum.txt')
times = data[:, 0]
body_ids = data[:, 1]
momenta = data[:, 2]

# Разделяем по телам
plt.figure(figsize=(10, 6))
for body_id in [0, 1]:
    mask = body_ids == body_id
    plt.plot(times[mask], momenta[mask], label=f'Body {body_id}')

plt.xlabel('время в с')
plt.ylabel('модуль момента имульса')
plt.title('сохранение момента импульса')
plt.legend()
plt.grid(True)
plt.savefig('angular_momentum.png')
plt.show()