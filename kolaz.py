from PIL import Image
import numpy as np


class Comparator(object):
    def __init__(self):
        self.img_files = ['img_le.jpg', 'img_ri.jpg']
        self.imgs = []
        self.img_arrays = []
        self.flat_imgs_array= []
        self._prepare()

    def _prepare(self):
        self._load_images()
        self.n = self._factorize()
        self._flatten_images()

    def get_flat_img(self, index):
        return Image.fromarray(self.flat_imgs_array[index])

    def _load_images(self):
        for i in range(len(self.img_files)):
            self.imgs.append(Image.open(self.img_files[i]).convert('L'))
            self.img_arrays.append(np.array(self.imgs[i], dtype=np.float))

    def _factors(self, n):
        return set(reduce(list.__add__, ([i, n//i] for i in range(1, int(n**0.5) + 1) if n % i == 0)))

    def _factorize(self):
        interni_faktori = []
        for img in self.imgs:
            interni_faktori.append(set(self._factors(img.size[0])).intersection(set(self._factors(img.size[1]))))
        kombo_faktori = list(set.intersection(*interni_faktori))
        razlika_16 = [abs(x - 16) for x in kombo_faktori]
        return kombo_faktori[razlika_16.index(min(razlika_16))]

    def _flatten_images(self):
        n = self.n
        # @TODO: svaki blok treba presikati u 1x1, prilagoditi slider
        for i in range(len(self.img_arrays)):
            self.flat_imgs_array.append(np.ones_like(self.img_arrays[i]))
            for cr in range(self.imgs[i].size[1] / n):
                for cc in range(self.imgs[i].size[0] / n):
                    self.flat_imgs_array[i][cr * n:cr * n + n, cc * n:cc * n + n] = np.sum(self.img_arrays[i][cr * n:cr * n + n, cc * n:cc * n + n]) / n**2

    def slide(self, master, slave):
        n = self.n
        min_master = None
        min_slave = None
        min_sum = 9999999999999
        x, y = 0, 0
        for cr in range(self.imgs[master].size[1] / n):
            for cc in range(self.imgs[master].size[0] / n):
                # racuna dimenzije sliding prozora za poredjenje
                w = self.imgs[master].size[0] - cc * n
                # w = w if self.imgs[slave].size[0] < w else self.imgs[slave].size[0]
                h = self.imgs[master].size[1] - cr * n
                # h = h if self.imgs[slave].size[1] < h else self.imgs[slave].size[1]
                master_slice = self.flat_imgs_array[master][cr * n:cr * n + h, cc * n:cc * n + w]
                slave_slice = self.flat_imgs_array[slave][:h, :w]
                # Image.fromarray(master_slice - slave_slice).show()
                # print h, w, np.sum(master_slice - slave_slice)
                tmp_sum = np.sum(abs(master_slice - slave_slice)) / (h * w)
                if tmp_sum < min_sum:
                    min_sum = tmp_sum
                    min_master = master_slice
                    min_slave = slave_slice
                    x, y = cc * n, cr * n
        kolaz_y = self.imgs[master].size[1] + self.imgs[slave].size[1] - len(min_slave)
        kolaz_x = self.imgs[master].size[0] + self.imgs[slave].size[0] - len(min_slave[0])
        kolaz = np.zeros((kolaz_y, kolaz_x))
        kolaz[:self.imgs[master].size[1], :self.imgs[master].size[0]] = self.img_arrays[master]
        kolaz[y:, x:] = self.img_arrays[slave]
        Image.fromarray(kolaz).show()
        # Image.fromarray(min_master).show()
        # Image.fromarray(min_slave).show()

c = Comparator()
c.slide(0, 1)
