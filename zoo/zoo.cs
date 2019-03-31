using System;
using System.Collections.Generic;

namespace ConsoleApp {

	public abstract class Animal {
		public int id;
		public int lifespan; // how long does it live
		public int pregnantPeriod; // how often does it have child
		public int years;
		public bool dead;

		public Animal(int id, int lifespan, int pregnantPeriod) {
			this.id = id;
			this.lifespan = lifespan;
			this.pregnantPeriod = pregnantPeriod;
			this.years = 0;
			this.dead = false;
		}

		public void celebrateBday() {
			this.years++;
		}

		public virtual void beDead() {
			this.dead = true;
		}

		public virtual void haveChild() {}

		public virtual void info() {}
	}

	public class Tiger: Animal {
		public Tiger(int id, int lifespan, int pregnantPeriod) : base(id, lifespan, pregnantPeriod) {}

		public override void beDead() {
			base.beDead();
			Console.WriteLine("Tiger " + id + " passed away");
		}

		public override void haveChild() {
			Console.WriteLine("Tiger " + id + " has a new child");
		}

		public override void info() {
			Console.WriteLine(years + " years old tiger number " + id);
		}
	}

	public class Zebra: Animal {
		public Zebra(int id, int lifespan, int pregnantPeriod) : base(id, lifespan, pregnantPeriod) {}

		public override void beDead() {
			base.beDead();
			Console.WriteLine("Zebra " + id + " passed away");
		}

		public override void haveChild() {
			Console.WriteLine("Zebra " + id + " has a new child");
		}

		public override void info() {
			Console.WriteLine(years + " years old zebra number " + id);
		}
	}

	public class Monkey: Animal {
		public Monkey(int id, int lifespan, int pregnantPeriod) : base(id, lifespan, pregnantPeriod) {}

		public override void beDead() {
			base.beDead();
			Console.WriteLine("Monkey " + id + " passed away");
		}

		public override void haveChild() {
			Console.WriteLine("Monkey " + id + " has a new child");
		}

		public override void info() {
			Console.WriteLine(years + " years old monkey number " + id);
		}
	}

	public class Zoo {
		public static void Main(string[] args) {
			List < Animal > animals = new List < Animal > ();
			animals.Add(new Tiger(0, 30, 20));
			animals.Add(new Zebra(1, 20, 15));
			animals.Add(new Monkey(2, 10, 6));

			int freeId = 3;

			List < Animal > childs = new List < Animal > ();

			int year = 0;
			// simulation of 50 years in Zoo
			while (year < 50) {
				Console.WriteLine("Year " + year);
				foreach(var animal in animals) {
					if (animal.dead == true) continue;

					if (animal.years >= animal.lifespan) {
						animal.beDead();
						continue;
					}

					if (animal.years > 0 && animal.years % animal.pregnantPeriod == 0) {
						animal.haveChild();

						int childId = freeId;
						freeId++;
						int childLifespan = animal.lifespan;
						int childPregnantPeriod = animal.pregnantPeriod;

						if (animal is Tiger) {
							childs.Add(new Tiger(childId, childLifespan, childPregnantPeriod));
						}
						else if (animal is Zebra) {
							childs.Add(new Zebra(childId, childLifespan, childPregnantPeriod));
						}
						else if (animal is Monkey) {
							childs.Add(new Monkey(childId, childLifespan, childPregnantPeriod));
						}
						else {
							Console.WriteLine("Unexpected animal occured in our zoo.");
						}
					}

					animal.celebrateBday();
				}

				animals.AddRange(childs);
				childs.Clear();

				year++;
			}

			Console.WriteLine("_______________________________________");
			Console.WriteLine("After 50 years in zoo there are:");
			foreach(var animal in animals) {
				if (animal.dead == false) {
					animal.info();
				}
			}
		}
	}
}
