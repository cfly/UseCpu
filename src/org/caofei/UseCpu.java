package org.caofei;

public class UseCpu {

	public static void main(String[] args) {
		if (args.length != 2) {
			System.out.println("usage java org.caofei.Java cpu% second");
			System.out.println("excemp: java org.caofei.Java 15 60");
			System.exit(-1);
		}
		if (!args[0].matches("\\d{1,2}")) {
			System.out.println("cpu percent is number");
			System.exit(-2);
		}
		int c = Integer.parseInt(args[0]);
		if (c < 0 && c > 100) {
			System.out.println("cpu percent range [0-100]");
			System.exit(-3);
		}
		if (!args[1].matches("\\d+")) {
			System.out.println("second is number");
			System.exit(-4);
		}
		int second = Integer.parseInt(args[1]);
		usecpu(c, second);
	}

	private static int COUNT = 1;
	private static int INTERVAL = 100;

	private static int usecpu(final int c, final int second) {
		System.out.println("expect\tcpu percent:" + c + "\tsecond:" + second);
		int availableProcessors = Runtime.getRuntime().availableProcessors();
		System.out.println("availableProcessors:"+availableProcessors);
		for (int i = 0; i < availableProcessors; i++) {
			new Thread() {
				@Override
				public void run() {
					long threadStartTime = System.currentTimeMillis();
					int i;
					int busySpan[] = new int[COUNT];
					int idleSpan[] = new int[COUNT];
					long startTime;
					for (i = 0; i < COUNT; i++) {
						busySpan[i] = (int) (INTERVAL * (((double) c) / 100));
						idleSpan[i] = INTERVAL - busySpan[i];
					}
					i = 0;
					while ((System.currentTimeMillis() - threadStartTime) < second * 1000) {
						i %= COUNT;
						startTime = System.currentTimeMillis();
						while ((System.currentTimeMillis() - startTime) <= busySpan[i])
							;
						try {
							Thread.sleep(idleSpan[i]);
						} catch (InterruptedException e) {
							e.printStackTrace();
							Thread.currentThread().interrupt();
						}
						i++;
					}
				}
			}.start();
		}
		try {
			Thread.sleep(second*1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
			Thread.currentThread().interrupt();
		}
		System.out.println("process end");
		return 0;
	}
}
